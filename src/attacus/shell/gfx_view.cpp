#include <string>
#include <iostream>

#include <spdlog/spdlog.h>

#include "SDL.h"
#include "SDL_syswm.h"

#include "gfx_view.h"

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
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    if (current_context_)
    {
      SDL_GL_MakeCurrent(sdl_window_, current_context_);
      context = SDL_GL_CreateContext(sdl_window_);
    }
    else
    {
      context = SDL_GL_CreateContext(sdl_window_);
    }
    current_context_ = context;
    if (context == NULL)
    {
      spdlog::error("Can't create opengl context: {}\n", SDL_GetError());
      return nullptr;
    }

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