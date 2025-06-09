#ifndef OGL_RENDER_BACKEND_H
#define OGL_RENDER_BACKEND_H

#include "../IRenderBackend.h"

class OGLRenderBackend : public IRenderBackend {
public:
    void Init(void* windowHandle) override;

    void Shutdown() override;

    void BeginFrame() override;

    void DrawMesh(const Renderable& renderUnit) override;

    void EndFrame() override;
};

#endif