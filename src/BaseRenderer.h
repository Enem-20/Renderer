#pragma once

#ifndef BASE_RENDERER
#define BASE_RENDERER

#include "Resources/ResourceBase.h"
#include "API/ExportPropety.h"

#include <glm/glm.hpp>

#include <queue>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <span>
#include <unordered_map>

class Sprite;
class Mesh;
class VertexBuffer;
class IndexBuffer;
class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class BaseTexture2D;

struct GLFWwindow;

//template<class DerivedRendererType>
class DLLEXPORT BaseRenderer : public ResourceBase/*<DerivedRendererType>*/ {
public:
	BaseRenderer(std::string_view name/*, DerivedRendererType* derivedObject*/) 
		: ResourceBase(name/*, derivedObject*/), ViewportSize(glm::vec2(0,0))
	{}
	virtual ~BaseRenderer(){}

	

	virtual void render() = 0;
	virtual void drawFrame() = 0;

	virtual void initWindow() = 0;

	virtual std::shared_ptr<BaseTexture2D> createTexture(std::string_view name, const std::string& relativePath, const uint32_t& texWidth, const uint32_t& texHeight, unsigned char* pixels, const uint32_t& texChannels = 4) = 0;
	virtual void addTexture(std::shared_ptr<BaseTexture2D> texture) = 0;
	virtual void removeTexture(std::string_view name) = 0;

	virtual void OnBeforeFrame()= 0;

	virtual bool windowShouldClose() const;
	virtual void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) = 0;
	virtual void afterComplete() = 0;

	GENERATETYPE(BaseRenderer)

	bool framebufferResized = false;
	uint32_t currentFrame = 0;
	
protected:
	/////////////////////////Placed in deleting order/////////////////////////

	/////////////////////////////////////////////////////////////////////////

	///////////////////////////Without deleting order////////////////////////

	////////////////////////////////////////////////////////////////////////
	std::vector<std::shared_ptr<BaseTexture2D>> textures{};
	glm::vec2 ViewportSize;
	std::queue<std::function<void()>> beforeFrameEventListeners;

};
#endif