#ifndef I_RENDER_BACKEND_H
#define I_RENDER_BACKEND_H

#include "Renderable.h"
#include <glm/glm.hpp>

class Mesh;
class Material;
class RenderUnit;

class IRenderBackend {
public:
    virtual ~IRenderBackend() = default;

    virtual void Init(void* windowHandle) = 0;

    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;

    virtual void DrawMesh(const Renderable& renderUnit) = 0;

    virtual void EndFrame() = 0;
};

#endif