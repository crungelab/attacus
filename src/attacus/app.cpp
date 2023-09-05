#include <string>
#include <iostream>
#include <chrono>

#include <spdlog/spdlog.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_syswm.h>
#include "SDL_mouse.h"

#include "app.h"

namespace attacus
{

    App::App(WindowParams params, App *parent) : GfxWindow(*parent, params),
                                                 state_(State::kRunning)
    {
        spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    }

    App::~App()
    {
    }

    void App::Create()
    {
        //SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "kmsdrm");
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland");
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");
        SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
        
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize video. SDL_Error: %s\n", SDL_GetError());
            return;
        }
        /*if (SDL_GL_LoadLibrary(nullptr) < 0)
        {
            printf("SDL_GL_LoadLibrary failed. SDL_Error: %s\n", SDL_GetError());
            return;
        }*/
        if (SDL_GL_LoadLibrary("libGLESv2.so") < 0) {
            printf("Unable to load OpenGL ES library: %s\n", SDL_GetError());
            return;
        }
        GfxWindow::Create();

        //RegisterResizer();
    }

    void App::Destroy()
    {
        GfxWindow::Destroy();
    }

    void App::Reset(ResetKind kind)
    {
        if (children_.size() == 0)
        {
            return;
        }
        if (children_[0]->view_id_ != 0)
        {
            return;
        }

        /*for (std::vector<View*>::iterator it = children_.begin(); it != children_.end(); ++it){
            View* child = *it;
            child->Reset();
        }*/

        GfxWindow::Reset(kind);
    }

    bool App::Dispatch(SDL_Event &event)
    {
        if (state_ == State::kShutdown)
            return false;

        if (event.window.windowID != windowId())
        {
            WindowBase *child = windowMap_[event.window.windowID];
            if (child != nullptr)
                return windowMap_[event.window.windowID]->Dispatch(event);
        }

        capture_ = false;
        switch (event.type)
        {
        case SDL_EVENT_USER:
        {
            Delegate &delegate = *static_cast<Delegate *>(event.user.data1);
            delegate();
            delete &delegate;
            event.user.data1 = nullptr;
            break;
        }
        case SDL_EVENT_KEY_DOWN:
        {
            int key = event.key.keysym.scancode;
            if (key == SDL_SCANCODE_F11)
            {
                capture_ = true;
                return true;
            }
        }
        }
        return GfxWindow::Dispatch(event);
    }

    using namespace std::chrono_literals;

    int App::Run()
    {
        // TODO:  Works but doesn't solve the problem of draw on move/resize
        /*
        render_thread_ = std::thread( [this] () -> void {
            while (this->state_ == App::State::kRunning) {
                App::PushCallbackEvent(new Delegate([this]() -> void {
                    this->Render();
                    }), this);
                std::this_thread::sleep_for(30ms);
            }
        });
        render_thread_.detach();
        */

        GfxWindow::Run();

        SDL_Quit();

        return 0;
    }

    static int EventWatcher(void *data, SDL_Event *event)
    {
        if (event->type == SDL_EVENT_WINDOW_RESIZED)
        {
            SDL_Window *win = SDL_GetWindowFromID(event->window.windowID);
            Window *window = static_cast<Window *>(SDL_GetWindowData(win, "Window"));
            if (window == nullptr)
            {
                return 0;
            }
            window->Dispatch(*event);
        }
        return 0;
    }

    void App::OnSize()
    {
        GfxWindow::OnSize();
        Render();
    }

    void App::RegisterResizer()
    {
        SDL_AddEventWatch(EventWatcher, this);
    }

} // namespace attacus