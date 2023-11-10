#pragma once

#ifndef RENDERER
#define RENDERER

#include "Resources/ResourceBase.h"
#include "API/ExportPropety.h"



#include <glm/glm.hpp>

#include <memory>
#include <functional>
#include <queue>
#include <span>
#include <string>
#include <vector>

class Sprite;
class Instance;
class DebugMessenger;
class WindowSurface;
class PhysicalDevice;
class LogicalDevice;
class SwapChain;
class RenderPipeline;
class RenderPass;
class CommandPool;
class Texture2D;
class Mesh;
class VertexBuffer;
class IndexBuffer;
class UniformBuffers;
class DescriptorPool;
class DescriptionSets;
class CommandBuffers;
class SyncObjects;
class DescriptorSetLayout;

struct GLFWwindow;

class DLLEXPORT Renderer : public ResourceBase{
public:
	Renderer(const std::string& name);
	~Renderer();

	void render();
	void drawFrame();

	void initWindow();

	void addTexture(std::shared_ptr<Texture2D> texture);
	void removeTexture(const std::string& name);

	void recreatePipeline(const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners = {}, std::vector<std::function<void()>> onAfterListeners = {});

	void OnBeforeFrame();

	GENERATETYPE(Renderer)

		static inline bool framebufferResized = false;
		static inline uint32_t currentFrame = 0;
private:
	void recreatePipelineReal(const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners = {}, std::vector<std::function<void()>> onAfterListeners = {});
	/////////////////////////Placed in deleting order/////////////////////////
	std::shared_ptr<Instance> instance{};
	std::shared_ptr<WindowSurface> windowSurface{};
	std::shared_ptr<DebugMessenger> debugMessanger{};
	std::shared_ptr<LogicalDevice> logicalDevice{};
	std::shared_ptr<CommandPool> commandPool{};
	std::shared_ptr<SyncObjects> syncObjects{};
	std::shared_ptr<RenderPass> renderPass{};
	std::shared_ptr<RenderPipeline> renderPipeline{};
	std::shared_ptr<DescriptorPool> descriptorPool{};
	std::vector<std::shared_ptr<Texture2D>> textures{};
	std::shared_ptr<SwapChain> swapchain{};
	/////////////////////////////////////////////////////////////////////////

	///////////////////////////Without deleting order////////////////////////
	std::shared_ptr<DescriptionSets> descriptionSets{};
	std::shared_ptr<CommandBuffers> commandBuffers{};
	std::shared_ptr<PhysicalDevice> physicalDevice{};
	////////////////////////////////////////////////////////////////////////
	static glm::vec2 ViewportSize;
	std::queue<std::function<void()>> beforeFrameEventListeners;
};
#endif