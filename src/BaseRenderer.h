#pragma once

#ifndef BASE_RENDERER
#define BASE_RENDERER

#include "Resources/ResourceBase.h"
#include "API/ExportPropety.h"

#include <glm/glm.hpp>

#include <queue>
#include <functional>
#include <memory>

class Sprite;
class Texture2D;
class Mesh;
class VertexBuffer;
class IndexBuffer;

struct GLFWwindow;

class DLLEXPORT BaseRenderer : public ResourceBase {
public:
	BaseRenderer(const std::string& name) : ResourceBase(name){}
	virtual ~BaseRenderer(){}

	virtual void render() = 0;
	virtual void drawFrame() = 0;

	virtual void initWindow() = 0;

	virtual void addTexture(std::shared_ptr<Texture2D> texture) = 0;
	virtual void removeTexture(const std::string& name) = 0;

	virtual void OnBeforeFrame()= 0;

	GENERATETYPE(BaseRenderer)

	bool framebufferResized = false;
	uint32_t currentFrame = 0;
protected:
	/////////////////////////Placed in deleting order/////////////////////////

	/////////////////////////////////////////////////////////////////////////

	///////////////////////////Without deleting order////////////////////////

	////////////////////////////////////////////////////////////////////////
	glm::vec2 ViewportSize;
	std::queue<std::function<void()>> beforeFrameEventListeners;
#endif
};
