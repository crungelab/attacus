#include <string>
#include <iostream>

#include <spdlog/spdlog.h>

#include <glad/gl.h>
//#include <glad/egl.h>
#include "SDL.h"
#include "SDL_syswm.h"
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

    gl_proc_resolver_ = (GLADloadfunc)SDL_GL_GetProcAddress;
    int version = gladLoadGL(gl_proc_resolver_);
    spdlog::info("OpenGL {}.{} loaded\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    //SDL_EGL_LoadLibraryOnly
      //int version = gladLoadEGLLoader((GLADloadproc)gl_proc_resolver_);
    //gladLoadEGLLoader((GLADloadproc)gl_proc_resolver_);
    //gladLoadEGLLoader((GLADloadproc)SDL_EGL_GetProcAddress);
    //spdlog::info("EGL {}.{} loaded\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    SDL_SysWMinfo wmInfo;
    SDL_GetWindowWMInfo(view().sdl_window_, &wmInfo, SDL_SYSWM_CURRENT_VERSION);

    EGLNativeDisplayType nativeDisplay = (EGLNativeDisplayType)wmInfo.info.x11.display;
    /*if (!gladLoadEGL()) {
        spdlog::error("Could not load EGL");
    }*/
    //int egl_version = gladLoadEGL(nativeDisplay, (GLADloadfunc)SDL_EGL_GetProcAddress);
    //spdlog::info("EGL {}.{} loaded\n", GLAD_VERSION_MAJOR(egl_version), GLAD_VERSION_MINOR(egl_version));

    //EGLDisplay display = eglGetDisplay(nativeDisplay);
    EGLDisplay display = eglGetPlatformDisplay(EGL_PLATFORM_X11_EXT, nativeDisplay, NULL);
    //EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
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