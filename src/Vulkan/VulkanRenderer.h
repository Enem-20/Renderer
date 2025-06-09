#pragma once

#include "../BaseRenderer.h"

#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

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

class DLLEXPORT VulkanRenderer : public BaseRenderer {
public:
	VulkanRenderer(const std::string& name);
	~VulkanRenderer() override;

	void render() override;
	void drawFrame() override;

	void initWindow() override;

	std::shared_ptr<Texture2D> createWindow(std::string_view name, const std::string& relativePath, int texWidth, int texHeight, int texChannels, unsigned char* pixels) override;
	void addTexture(std::shared_ptr<Texture2D> texture) override;
	void removeTexture(const std::string& name) override;

	void recreatePipeline(const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners = {}, std::vector<std::function<void()>> onAfterListeners = {});

	void OnBeforeFrame() override;
	void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) override{}

	GENERATETYPE(VulkanRenderer)

	std::shared_ptr<Instance> getInstance() const {
		return instance;
	}
	std::shared_ptr<LogicalDevice> getLogicalDevice() const {
		return logicalDevice;
	}
	std::shared_ptr<CommandBuffers> getCommandBuffers() const {return commandBuffers;};
	std::shared_ptr<RenderPipeline> getRenderPipeline() const {
		return renderPipeline;
	}
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
};


#endif // !VULKAN_RENDERER
