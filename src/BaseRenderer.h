#pragma once

#ifndef BASE_RENDERER_H
#define BASE_RENDERER_H

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
	BaseRenderer(const std::string& name) : ResourceBase(name), ViewportSize(glm::vec2(0,0)){}
	virtual ~BaseRenderer(){}

	virtual void render() = 0;
	virtual void drawFrame() = 0;

	virtual void initWindow() = 0;

	virtual std::shared_ptr<Texture2D> createWindow(std::string_view name, const std::string& relativePath, int texWidth, int texHeight, int texChannels, unsigned char* pixels) = 0;

	virtual void addTexture(std::shared_ptr<Texture2D> texture) = 0;
	virtual void removeTexture(const std::string& name) = 0;

	virtual void OnBeforeFrame()= 0;

	virtual bool windowShouldClose() const;
	virtual void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) = 0;

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

class RendererContext {
	std::shared_ptr<BaseRenderer> rendererContext;

	void SetRenderer(std::shared_ptr<BaseRenderer> newRenderer) {
		rendererContext = newRenderer;
	}

	std::shared_ptr<BaseRenderer> getRenderer() {
		return rendererContext;
	}
};