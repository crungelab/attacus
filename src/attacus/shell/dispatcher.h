#pragma once

#include <SDL.h>

#include <attacus/core/component.h>

namespace attacus {

class Dispatcher : public Component {
public:
    virtual bool Dispatch(SDL_Event& event);
    //virtual bool DispatchWindowEvent(SDL_Event& event) { return true; }
    virtual void OnResize(SDL_Event& event) {}
    virtual void OnSize() { ResizeChildren(); }
    virtual void ResizeChildren() {}
    virtual void OnShow() {}

    static void PushCallbackEvent(Delegate* delegate, void* user_data);

};

} //namespace attacus
