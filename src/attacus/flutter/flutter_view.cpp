#include <cassert>
#include <chrono>
#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;

#include <string>

#include <spdlog/spdlog.h>

#include <SDL.h>

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

        SDL_GL_MakeCurrent(sdl_window_, nullptr);
    }

    void FlutterView::InitRendererConfig(FlutterRendererConfig &config)
    {
        config.type = kOpenGL;
        config.open_gl.struct_size = sizeof(config.open_gl);
        config.open_gl.make_current = [](void *userdata) -> bool
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            auto window = self.sdl_window_;
            SDL_GL_MakeCurrent(window, self.context_);
            return true;
        };
        config.open_gl.make_resource_current = [](void *userdata) -> bool
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            auto window = self.sdl_window_;
            SDL_GL_MakeCurrent(window, self.resource_context_);
            return true;
        };
        config.open_gl.clear_current = [](void *userdata) -> bool
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            auto window = self.sdl_window_;
            SDL_GL_MakeCurrent(window, nullptr);
            return true;
        };
        config.open_gl.present = [](void *userdata) -> bool
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            auto window = self.sdl_window_;
            SDL_GL_SwapWindow(window); // TODO:This presents a problem if the view is embedded
            return true;
        };
        config.open_gl.fbo_callback = [](void *userdata) -> uint32_t
        {
            return 0; // FBO0
        };
        // config.open_gl.fbo_reset_after_present = true;
        // config.open_gl.fbo_reset_after_present = false;

        config.open_gl.gl_proc_resolver = [](void *userdata, const char *name) -> void *
        {
            FlutterView &self = *static_cast<FlutterView *>(userdata);
            if (strncmp(name, "egl", 3) == 0) {
                return (void*)SDL_EGL_GetProcAddress(name);
            } else {
                //return (void*)self.gfx().gl_proc_resolver_(name);
                return (void*)SDL_GL_GetProcAddress(name);
            }
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
        args.custom_task_runners = &runner_->custom_task_runners;

        args.compositor = compositor().InitCompositor();
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
        //printf("Window size: width=%i, height=%i\n", w, h);
        int pw, ph;
        SDL_GetWindowSizeInPixels(sdl_window_, &pw, &ph);
        //printf("Window size in pixels: width=%i, height=%i\n", pw, ph);

        pixelRatio_ = SDL_GetWindowPixelDensity(sdl_window_) * zoom_ ;
        //spdlog::debug("Pixel ratio: {}", pixelRatio_);

        FlutterWindowMetricsEvent event = {0};
        event.struct_size = sizeof(event);
        event.width = pw;
        event.height = ph;
        event.pixel_ratio = pixelRatio_;

        FlutterEngineSendWindowMetricsEvent(engine_, &event);
        //FlutterEngineScheduleFrame(engine_);
    }

    bool FlutterView::Dispatch(SDL_Event &e)
    {
        mouseInput_->Dispatch(e);
        textInput_->Dispatch(e);

        return GfxView::Dispatch(e);
    }

} // namespace attacus