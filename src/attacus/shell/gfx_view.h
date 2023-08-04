#pragma once

#include "view.h"

namespace attacus {

class GfxView : public View {
public:
    GfxView(View& parent, ViewParams params = ViewParams());
    virtual void Create() override;
    virtual void* CreateContext();
    virtual void CreateGfx();
    void CreateFramebuffer() override;
    void Reset(ResetKind kind = ResetKind::kSoft) override;
    void OnSize() override;

    virtual void PreRender() override;
    virtual void PostRender() override;

    // Accessors
    // Data members
    static void* current_context_;
    static void* gfx_context_;

    int64_t time_offset_;
    bool capture_ = false;

};

} // namespace attacus