#pragma once

#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"

#include <memory>
#include <vector>

class PhysicalDevice;
class WindowSurface;
class SwapChain;
class CommandBuffer;
class CommandBuffers;
class CommandPool;
class SyncObjects;
class RenderPass;

struct VkDevice_T;
typedef VkDevice_T* VkDevice;
struct VkQueue_T;
typedef VkQueue_T* VkQueue;


typedef uint64_t VkDeviceSize;
typedef uint32_t VkFlags;
typedef VkFlags VkMemoryPropertyFlags;//
typedef VkFlags VkBufferUsageFlags;

struct VkImage_T;
struct VkImageView_T;
struct VkDeviceMemory_T;
struct VkBuffer_T;//
typedef VkImage_T* VkImage;
typedef VkImageView_T* VkImageView;
typedef VkDeviceMemory_T* VkDeviceMemory;
typedef VkBuffer_T* VkBuffer;


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

	void queuePresent(SwapChain& swapchain, RenderPass& renderPass, CommandBuffers& commandBuffers, CommandPool& commandPool, SyncObjects& syncObjects, uint32_t currentFrame, uint32_t imageIndex, bool framebufferResized);
	void queueSubmitForSingleBuffer(CommandBuffer& commandBuffer);
	void queueWaitIdleGraphics();

	GENERATETYPE(LogicalDevice)
private:
	PhysicalDevice& physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};

#endif