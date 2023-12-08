#pragma once

#include "../BaseRenderer.h"

#ifndef VULKAN_RENDERER
#define VULKAN_RENDERER

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
class RenderPipeline;
class RenderPass;
class Mesh;
class VertexBuffer;
class IndexBuffer;
class UniformBuffers;
class DescriptorPool;
class DescriptionSets;
class CommandBuffers;
class SyncObjects;
class DescriptorSetLayout;
class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class BaseTexture2D;

struct GLFWwindow;

class DLLEXPORT VulkanRenderer : public BaseRenderer/*<VulkanRenderer>*/ {
public:
	VulkanRenderer(const std::string& name);
	~VulkanRenderer() override;

	void render() override;
	void drawFrame() override;

	void initWindow() override;

	std::shared_ptr<BaseTexture2D> createTexture(std::string_view name, const std::string& relativePath, const uint32_t& texWidth, const uint32_t& texHeight, unsigned char* pixels, const uint32_t& texChannels = 4) override;
	void addTexture(std::shared_ptr<BaseTexture2D> texture) override;
	void removeTexture(std::string_view name) override;

	void recreatePipeline(const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners = {}, std::vector<std::function<void()>> onAfterListeners = {});



	bool windowShouldClose() const override;
	void OnBeforeFrame() override;
	void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) override {}

	void afterComplete() override;

	std::shared_ptr<Instance> getInstance() const;
	std::shared_ptr<WindowSurface> getWindowSurface() const;
	std::shared_ptr<DebugMessenger> getDebugMessanger() const;
	std::shared_ptr<LogicalDevice> getLogicalDevice() const;
	std::shared_ptr<CommandPool> getCommandPool() const;
	std::shared_ptr<SyncObjects> getSyncObjects() const;
	std::shared_ptr<RenderPass> getRenderPass() const;
	std::shared_ptr<RenderPipeline> getRenderPipeline() const;
	std::shared_ptr<DescriptorPool> getDescriptorPool() const;
	std::vector<std::shared_ptr<BaseTexture2D>> getTextures() const;
	std::shared_ptr<SwapChain> getSwapchain() const;
	

	std::shared_ptr<DescriptionSets> getDescriptionSets() const;
	std::shared_ptr<CommandBuffers> getCommandBuffers() const;
	std::shared_ptr<PhysicalDevice> getPhysicalDevice() const;

	GENERATETYPE(VulkanRenderer)
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
	std::vector<std::shared_ptr<BaseTexture2D>> textures{};
	std::shared_ptr<SwapChain> swapchain{};
	/////////////////////////////////////////////////////////////////////////

	///////////////////////////Without deleting order////////////////////////
	std::shared_ptr<DescriptionSets> descriptionSets{};
	std::shared_ptr<CommandBuffers> commandBuffers{};
	std::shared_ptr<PhysicalDevice> physicalDevice{};
	////////////////////////////////////////////////////////////////////////
};


#endif // !VULKAN_RENDERER
