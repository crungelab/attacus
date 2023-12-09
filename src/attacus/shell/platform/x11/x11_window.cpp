#include <cstring>

#include "SDL.h"

#include "x11_window.h"

namespace attacus {

X11Window::~X11Window() {
}

XWindow X11Window::GetNativeHandle() {
    XWindow hwnd = (XWindow)SDL_GetProperty(SDL_GetWindowProperties(sdl_window_), "SDL.window.x11.window", 0);
    return hwnd;
}

} //namespace attacus