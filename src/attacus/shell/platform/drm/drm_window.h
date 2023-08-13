#pragma once

#include <functional>
#include <memory>
#include <string>

#include <attacus/shell/window_base.h>


namespace attacus {

class DrmWindow : public WindowBase {
 public:

    DrmWindow(WindowBase& parent, WindowParams params = WindowParams()) : WindowBase(parent, params) {}
    virtual ~DrmWindow();
};

} //namespace fulkan