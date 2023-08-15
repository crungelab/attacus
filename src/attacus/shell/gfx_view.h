#pragma once

//#include <glad/glad.h>
#include "view.h"

namespace attacus {

typedef void* (* GLESloadproc)(const char *name);

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
    void* current_context_;
    void* gfx_context_;
    GLESloadproc gl_proc_resolver_ = nullptr;

    int64_t time_offset_;
    bool capture_ = false;

};

} // namespace attacus