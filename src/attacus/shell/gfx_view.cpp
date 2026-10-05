#include <string>
#include <iostream>

#include <spdlog/spdlog.h>

#include "SDL.h"

#include "gfx_view.h"

namespace attacus
{

  SDL_GLContext GfxView::current_context_ = nullptr;
  SDL_GLContext GfxView::gfx_context_ = nullptr;

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

  SDL_GLContext GfxView::CreateContext()
  {
    if (!SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1))
    {
      spdlog::error("Can't set share attribute: {}", SDL_GetError());
    }

    if (current_context_ && !SDL_GL_MakeCurrent(sdl_window_, current_context_))
    {
      spdlog::error("Can't make context current: {}", SDL_GetError());
      return nullptr;
    }

    SDL_GLContext context = SDL_GL_CreateContext(sdl_window_);
    if (!context)
    {
      spdlog::error("Can't create OpenGL context: {}", SDL_GetError());
      return nullptr;
    }

    current_context_ = context;
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
    if (SDL_GL_SetSwapInterval(1))
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