#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

#include <filesystem>
namespace fs = std::filesystem;

#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <glad/glad_egl.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_syswm.h>
// #include <SDL3/SDL_egl.h>
// #include <SDL3/SDL_opengles2.h>

#include <attacus/app.h>

#include "flutter_embedder.h"
#include "flutter_messenger.h"
#include "flutter_runner.h"
#include "flutter_view.h"

// TODO: Fix:  Depends on OpenGL
#include "compositor/gl/compositor_gl.h"

#include "components/cursor.h"
#include "components/mouse_input.h"
#include "components/text_input.h"

#include "components/texture_registrar.h"
#include "components/view_registry.h"

// Maximum damage history - for triple buffering we need to store damage for
// last two frames; Some Android devices (Pixel 4) use quad buffering.
static const int kMaxHistorySize = 10;

// Keeps track of the most recent frame damages so that existing damage can
// be easily computed.
std::list<FlutterRect> damage_history_;

// Keeps track of the existing damage associated with each FBO ID
std::unordered_map<intptr_t, FlutterRect *> existing_damage_map_;

EGLDisplay display_;
EGLSurface surface_;

// Auxiliary function used to transform a FlutterRect into the format that is
// expected by the EGL functions (i.e. array of EGLint).
static std::array<EGLint, 4> RectToInts(const FlutterRect rect)
{
    EGLint height;
    eglQuerySurface(display_, surface_, EGL_HEIGHT, &height);

    std::array<EGLint, 4> res{
        static_cast<int>(rect.left), height - static_cast<int>(rect.bottom),
        static_cast<int>(rect.right) - static_cast<int>(rect.left),
        static_cast<int>(rect.bottom) - static_cast<int>(rect.top)};
    return res;
}

// Auxiliary function to union the damage regions comprised by two FlutterRect
// element. It saves the result of this join in the rect variable.
static void JoinFlutterRect(FlutterRect *rect, FlutterRect additional_rect)
{
    rect->left = std::min(rect->left, additional_rect.left);
    rect->top = std::min(rect->top, additional_rect.top);
    rect->right = std::max(rect->right, additional_rect.right);
    rect->bottom = std::max(rect->bottom, additional_rect.bottom);
}

// Auxiliary function used to check if the given list of extensions contains the
// requested extension name.
static bool HasExtension(const char *extensions, const char *name)
{
    const char *r = strstr(extensions, name);
    auto len = strlen(name);
    // check that the extension name is terminated by space or null terminator
    return r != nullptr && (r[len] == ' ' || r[len] == 0);
}

namespace attacus
{

    FlutterView::FlutterView(View &parent, FlutterConfig &config, ViewParams params) : GfxView(parent, params)
    {
        config_ = &config;
        messenger_ = new FlutterMessenger(*this);
        runner_ = new FlutterRunner(*this);

        compositor_ = new CompositorGL(*this);

        cursor_ = new CursorComponent(*this);
        mouseInput_ = new MouseInput(*this);
        textInput_ = new TextInput(*this);

        textureRegistrar_ = new TextureRegistrar(*this);
        viewRegistry_ = new ViewRegistry(*this);

        // Get the display and surface variables.
        display_ = SDL_EGL_GetCurrentEGLDisplay();
        surface_ = SDL_EGL_GetWindowEGLSurface(sdl_window_);

    }

    FlutterView::~FlutterView()
    {
    }

    void FlutterView::CreateGfx()
    {
        GfxView::CreateGfx();

        resource_context_ = CreateContext();
        if (resource_context_ == NULL)
        {
            spdlog::error("Can't create opengl context for resource window: {}", SDL_GetError());
            return;
        }

        context_ = CreateContext();
        if (context_ == NULL)
        {
            spdlog::error("Can't create opengl context: {}", SDL_GetError());
            return;
        }
        if (SDL_GL_MakeCurrent(sdl_window_, nullptr) < 0)
        {
            spdlog::error("Can't unset opengl context: {}", SDL_GetError());
            return;
        }
    }

    void FlutterView::InitRendererConfig(FlutterRendererConfig &config)
    {
        config.type = kOpenGL;
        config.open_gl.struct_size = sizeof(config.open_gl);
        config.open_gl.make_current = [](void *userdata) -> bool
        {
            // spdlog::debug("make_current");
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            auto window = self.sdl_window_;
            if (SDL_GL_MakeCurrent(window, self.context_) < 0)
            {
                spdlog::error("Can't set opengl context: {}", SDL_GetError());
                return false;
            }
            return true;
        };
        /*config.open_gl.make_resource_current = [](void *userdata) -> bool
        {
            spdlog::debug("make_resource_current");
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            auto window = self.sdl_window_;
            if (SDL_GL_MakeCurrent(window, self.resource_context_) < 0)
            {
                spdlog::error("Can't set opengl resource context: {}", SDL_GetError());
                return false;
            }
            return true;
        };*/
        config.open_gl.clear_current = [](void *userdata) -> bool
        {
            spdlog::debug("clear_current");
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            auto window = self.sdl_window_;
            if (SDL_GL_MakeCurrent(window, nullptr) < 0)
            {
                spdlog::error("Can't unset opengl context: {}", SDL_GetError());
                return false;
            }
            return true;
        };
        /*config.open_gl.present = [](void *userdata) -> bool
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            return SDL_GL_SwapWindow(self.sdl_window_);
        };*/
        config.open_gl.present_with_info =
            [](void *userdata, const FlutterPresentInfo *info) -> bool
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            // Free the existing damage that was allocated to this frame.
            if (existing_damage_map_[info->fbo_id] != nullptr)
            {
                free(existing_damage_map_[info->fbo_id]);
                existing_damage_map_[info->fbo_id] = nullptr;
            }

            // Get list of extensions.
            const char *extensions = eglQueryString(display_, EGL_EXTENSIONS);

            // Retrieve the set damage region function.
            PFNEGLSETDAMAGEREGIONKHRPROC set_damage_region_ = nullptr;
            if (HasExtension(extensions, "EGL_KHR_partial_update"))
            {
                set_damage_region_ = reinterpret_cast<PFNEGLSETDAMAGEREGIONKHRPROC>(
                    SDL_EGL_GetProcAddress("eglSetDamageRegionKHR"));
            }

            // Retrieve the swap buffers with damage function.
            PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC swap_buffers_with_damage_ = nullptr;
            if (HasExtension(extensions, "EGL_EXT_swap_buffers_with_damage"))
            {
                swap_buffers_with_damage_ =
                    reinterpret_cast<PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC>(
                        SDL_EGL_GetProcAddress("eglSwapBuffersWithDamageEXT"));
            }
            else if (HasExtension(extensions, "EGL_KHR_swap_buffers_with_damage"))
            {
                swap_buffers_with_damage_ =
                    reinterpret_cast<PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC>(
                        SDL_EGL_GetProcAddress("eglSwapBuffersWithDamageKHR"));
            }

            if (set_damage_region_)
            {
                // Set the buffer damage as the damage region.
                auto buffer_rects = RectToInts(info->buffer_damage.damage[0]);
                set_damage_region_(display_, surface_, buffer_rects.data(), 1);
            }

            // Add frame damage to damage history
            damage_history_.push_back(info->frame_damage.damage[0]);
            if (damage_history_.size() > kMaxHistorySize)
            {
                damage_history_.pop_front();
            }

            if (swap_buffers_with_damage_)
            {
                // Swap buffers with frame damage.
                auto frame_rects = RectToInts(info->frame_damage.damage[0]);
                return swap_buffers_with_damage_(display_, surface_, frame_rects.data(),
                                                 1);
            }
            else
            {
                // If the required extensions for partial repaint were not provided, do
                // full repaint.
                return eglSwapBuffers(display_, surface_);
                //return SDL_GL_SwapWindow(self.sdl_window_);
            }
        };

        config.open_gl.populate_existing_damage =
            [](void *userdata, intptr_t fbo_id,
               FlutterDamage *existing_damage) -> void
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            // Given the FBO age, create existing damage region by joining all frame
            // damages since FBO was last used
            EGLint age;
            if (SDL_GL_ExtensionSupported("GL_EXT_buffer_age") == SDL_TRUE)
            {
                eglQuerySurface(display_, surface_, EGL_BUFFER_AGE_EXT, &age);
            }
            else
            {
                age = 4; // Virtually no driver should have a swapchain length > 4.
            }

            existing_damage->num_rects = 1;

            // Allocate the array of rectangles for the existing damage.
            existing_damage_map_[fbo_id] = static_cast<FlutterRect *>(
                malloc(sizeof(FlutterRect) * existing_damage->num_rects));
            existing_damage_map_[fbo_id][0] =
                FlutterRect{0, 0, (double)self.width(), (double)self.height()};
            existing_damage->damage = existing_damage_map_[fbo_id];

            if (age > 1)
            {
                --age;
                // join up to (age - 1) last rects from damage history
                for (auto i = damage_history_.rbegin();
                     i != damage_history_.rend() && age > 0; ++i, --age)
                {
                    if (i == damage_history_.rbegin())
                    {
                        if (i != damage_history_.rend())
                        {
                            existing_damage->damage[0] = {i->left, i->top, i->right, i->bottom};
                        }
                    }
                    else
                    {
                        JoinFlutterRect(&(existing_damage->damage[0]), *i);
                    }
                }
            }
        };

        config.open_gl.fbo_callback = [](void *userdata) -> uint32_t
        {
            return 0; // FBO0
        };
        
        config.open_gl.fbo_reset_after_present = true;
        // config.open_gl.fbo_reset_after_present = false;

        config.open_gl.gl_proc_resolver = [](void *userdata, const char *name) -> void *
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            // spdlog::debug("gl_proc_resolver: {}", name);
            SDL_FunctionPointer func;
            if (strncmp(name, "egl", 3) == 0)
            {
                func = SDL_EGL_GetProcAddress(name);
            }
            else
            {
                func = SDL_GL_GetProcAddress(name);
            }
            return (void *)func;
        };

        config.open_gl.gl_external_texture_frame_callback =
            [](void *userdata, int64_t texId, size_t width, size_t height, FlutterOpenGLTexture *texOut) -> bool
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            return self.textureRegistrar().CopyTexture(texId, width, height, texOut);
        };

        config.open_gl.populate_existing_damage = nullptr;
    }

    void FlutterView::InitProjectArgs(FlutterProjectArgs &args)
    {
        args.struct_size = sizeof(FlutterProjectArgs);
        args.assets_path = strdup(config().assets_path_.c_str());
        args.icu_data_path = strdup(config().icu_data_path_.c_str());
        args.platform_message_callback = [](const FlutterPlatformMessage *message, void *user_data)
        {
            FlutterView &self = *static_cast<FlutterView *>(user_data);
            self.messenger().Receive(*message);
        };
        // TODO: Flutter embedder has a bug using custom task runners in debug mode
        // https://github.com/flutter/flutter/issues/129533
        // args.custom_task_runners = &runner_->custom_task_runners;

        // args.compositor = compositor().InitCompositor();
    }

    void FlutterView::InitEngine(FlutterRendererConfig &config, FlutterProjectArgs &args)
    {
        FlutterEngineResult result = FlutterEngineInitialize(FLUTTER_ENGINE_VERSION, &config, &args, this, &engine_);
        if (result != kSuccess || engine_ == nullptr)
        {
            std::cout << "Could not initialize the Flutter Engine." << std::endl;
            return;
        }
        engine_api_.struct_size = sizeof(FlutterEngineProcTable);
        result = FlutterEngineGetProcAddresses(&engine_api_);
        if (result != kSuccess)
        {
            std::cout << "Could not get the Flutter Engine Procedure Table." << std::endl;
            return;
        }
    }

    void FlutterView::Create()
    {
        GfxView::Create();

        FlutterRendererConfig config = {};
        InitRendererConfig(config);
        FlutterProjectArgs args = {};
        InitProjectArgs(args);

        InitEngine(config, args);

        messenger().Create();
        runner().Create();

        compositor().Create();

        cursor().Create();
        mouseInput().Create();
        textInput().Create();

        textureRegistrar().Create();
        viewRegistry().Create();

        FlutterEngineResult result = FlutterEngineRunInitialized(engine_);
        if (result != kSuccess || engine_ == nullptr)
        {
            std::cout << "Could not run the Flutter Engine." << std::endl;
            return;
        }
    }

    void FlutterView::Destroy()
    {
        FlutterEngineResult result = FlutterEngineDeinitialize(engine_);
        Shutdown();
        GfxView::Destroy();
    }

    void FlutterView::OnResize(SDL_Event &event)
    {
        GfxView::OnResize(event);
        UpdateSize();
    }

    void FlutterView::OnSize()
    {
        GfxView::OnSize();
        UpdateSize();
    }

    void FlutterView::OnShow()
    {
        GfxView::OnShow();
        UpdateSize();
    }

    void FlutterView::UpdateSize()
    {
        int w, h;
        SDL_GetWindowSize(sdl_window_, &w, &h);
        // printf("Window size: width=%i, height=%i\n", w, h);
        int pw, ph;
        SDL_GetWindowSizeInPixels(sdl_window_, &pw, &ph);
        // printf("Window size in pixels: width=%i, height=%i\n", pw, ph);

        pixelRatio_ = SDL_GetWindowPixelDensity(sdl_window_) * zoom_;
        // spdlog::debug("Pixel ratio: {}", pixelRatio_);

        FlutterWindowMetricsEvent event = {0};
        event.struct_size = sizeof(event);
        event.width = pw;
        event.height = ph;
        event.pixel_ratio = pixelRatio_;

        FlutterEngineSendWindowMetricsEvent(engine_, &event);
        // FlutterEngineScheduleFrame(engine_);
    }

    bool FlutterView::Dispatch(SDL_Event &e)
    {
        mouseInput_->Dispatch(e);
        textInput_->Dispatch(e);

        return GfxView::Dispatch(e);
    }

} // namespace attacus