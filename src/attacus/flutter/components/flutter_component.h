#pragma once

#include <attacus/shell/view_component.h>

#include <flutter_embedder.h>

namespace attacus {

class FlutterView;

class FlutterComponent : public ViewComponent<FlutterView> {
public:
    FlutterComponent(FlutterView& view);
    FlutterView& flutter() { return *view_; }
    virtual void Create() override;
    // Data members
    FlutterEngine engine_;
};

} //namespace attacus