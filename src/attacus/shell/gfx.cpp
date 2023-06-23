#include <string>
#include <iostream>

#include <spdlog/spdlog.h>

#include "SDL.h"
#include "SDL_syswm.h"
#include "SDL_opengl.h"
#include "SDL_egl.h"

#include "gfx_view.h"
#include "gfx.h"

namespace attacus
{

  Gfx *Gfx::instance_ = nullptr;

  Gfx::Gfx(GfxView &view) : view_(&view),
                            resetFlags_(0),
                            debugFlags_(0)
  {
    instance_ = this;
  }

  void Gfx::Create()
  {
    SDL_SysWMinfo wmInfo;
    SDL_GetWindowWMInfo(view().sdl_window_, &wmInfo, SDL_SYSWM_CURRENT_VERSION);

    EGLNativeDisplayType nativeDisplay = (EGLNativeDisplayType)wmInfo.info.x11.display;
    EGLDisplay display = eglGetPlatformDisplay(EGL_PLATFORM_X11_EXT, nativeDisplay, NULL);
    EGLint major, minor;
    if (!eglInitialize(display, &major, &minor)) {
      spdlog::error("Could not initialize EGL");
    } else {
      spdlog::info("EGL {}.{} loaded\n", major, minor);
    }


    EGLConfig config;
    EGLint num_config;

    const EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,  // Use EGL_OPENGL_ES2_BIT for OpenGL ES2
        EGL_NONE
    };

    eglChooseConfig(display, config_attribs, &config, 1, &num_config);


    EGLNativeWindowType nativeWindow = (EGLNativeWindowType)wmInfo.info.x11.window;
    EGLSurface surface = eglCreateWindowSurface(display, config, nativeWindow, NULL);

    EGLContext context;
    EGLint context_attribs[] = { EGL_CONTEXT_MAJOR_VERSION, 4, 6 };  // OpenGL 4.6
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);

    eglMakeCurrent(display, surface, surface, context);


  }

  void Gfx::Reset()
  {
    // bgfx::reset(view().width(), view().height(), resetFlags_);
  }
} // namespace attacus