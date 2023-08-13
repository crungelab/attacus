#include <string>
#include <iostream>

#include <spdlog/spdlog.h>

#include "SDL.h"
#include "SDL_syswm.h"
#include <SDL_opengles2.h>

#include "gfx_view.h"

#include <SDL_opengles2.h>

namespace attacus
{

  void *GfxView::current_context_ = nullptr;
  void *GfxView::gfx_context_ = nullptr;

  GfxView::GfxView(View &parent, ViewParams params) : View(parent, params),
                                                      time_offset_(0)
  {
  }

  void GfxView::Create()
  {
    View::Create();
    CreateGfx();
  }

  void GfxView::PreRender()
  {
    View::PreRender();
    SDL_GL_MakeCurrent(sdl_window_, gfx_context_);
  }

  void GfxView::PostRender()
  {
    Touch();
    View::PostRender();
  }

  void *GfxView::CreateContext()
  {
    void *context = nullptr;
    //SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    if (current_context_)
    {
      if(SDL_GL_MakeCurrent(sdl_window_, current_context_) < 0)
      {
        spdlog::error("CreateContext:  Can't set opengl context: {}\n", SDL_GetError());
        return nullptr;
      }
      context = SDL_GL_CreateContext(sdl_window_);
    }
    else
    {
      context = SDL_GL_CreateContext(sdl_window_);
    }
    current_context_ = context;
    if (context == nullptr)
    {
      spdlog::error("Can't create opengl context: {}\n", SDL_GetError());
      return nullptr;
    }

    typedef const GLubyte *(*PFNGLGETSTRINGPROC)(GLenum name);
    PFNGLGETSTRINGPROC my_glGetString = (PFNGLGETSTRINGPROC)SDL_GL_GetProcAddress("glGetString");
    if (!my_glGetString)
    {
      printf("Failed to retrieve function pointer for glGetString.\n");
    }
    const GLubyte *version = my_glGetString(GL_VERSION);
    if (version)
    {
      printf("OpenGL Version: %s\n", version);
    }
    else
    {
      printf("Failed to retrieve OpenGL version.\n");
    }

    /*if(SDL_GL_MakeCurrent(sdl_window_, nullptr) < 0)
    {
      spdlog::error("CreateContext:  Can't clear opengl context: {}\n", SDL_GetError());
      return nullptr;
    }*/
    return context;
  }

  void GfxView::CreateGfx()
  {
    if (!gfx_context_)
    {
      gfx_context_ = CreateContext();
      if (gfx_context_ == nullptr)
      {
        return;
      }
    }
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
      spdlog::error("Couldn't enable vsync: {}\n", SDL_GetError());
    }
    if(SDL_GL_MakeCurrent(sdl_window_, nullptr) < 0)
    {
      spdlog::error("CreateContext:  Can't clear opengl context: {}\n", SDL_GetError());
    }
  }

  void GfxView::CreateFramebuffer()
  {
    View::CreateFramebuffer();
  }

  void GfxView::Reset(ResetKind kind)
  {
    CreateFramebuffer();
  }

  void GfxView::OnSize()
  {
    Reset(ResetKind::kHard);
  }

} // namespace attacus