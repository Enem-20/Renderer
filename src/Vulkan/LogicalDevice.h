#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include <GLFW\glfw3.h>

#include <memory>
#include <vector>

class PhysicalDevice;
class WindowSurface;
class SwapChain;
class CommandBuffer;
class CommandBuffers;
class CommandPool;
class SyncObjects;
class RenderPipeline;

class DLLEXPORT LogicalDevice : public ResourceBase{
public:
	LogicalDevice(const std::string& name, WindowSurface& windowSurface, PhysicalDevice& physicalDevice);
	LogicalDevice(const LogicalDevice& logicalDevice);
	VkDevice& getRaw();
	VkQueue& getGraphicsQueue();

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static void copyBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void wait();

	~LogicalDevice();

	void queuePresent(SwapChain& swapchain, RenderPipeline& renderPipeline, CommandBuffers& commandBuffers, CommandPool& commandPool, SyncObjects& syncObjects, uint32_t currentFrame, uint32_t imageIndex, bool framebufferResized);
	void queueSubmitForSingleBuffer(CommandBuffer& commandBuffer);
	void queueWaitIdleGraphics();

	GENERATETYPE(LogicalDevice)
private:
	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	PhysicalDevice& physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};