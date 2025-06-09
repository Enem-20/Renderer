#pragma once

#include "FrameBuffers.h"
#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H

#include <memory>
#include <vector>

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"
#include "SwapChain.h"

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
struct VkFence_T;
typedef VkFence_T* VkFence;

class Device;
class PhysicalDevice;
class WindowSurface;
class SwapChain;
class CommandBuffer;
class CommandBuffers;
class CommandPool;
class SyncObjects;
class RenderPass;
class SingleTimeBuffer;
class GPUQueue;

class DLLEXPORT LogicalDevice : public ResourceBase{
public:
	LogicalDevice(const std::string& name, Device* owner);
	LogicalDevice(const LogicalDevice& logicalDevice);
	VkDevice getRaw() const;
	Device* getOwner() const;
	GPUQueue* getGraphicsQueue() const;
	CommandPool* getCommandPool() const;

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	VkFence createFence() const;
	VkSemaphore createSemaphore() const;
	void createFrameBuffers();

	SingleTimeBuffer* createSingleTimeBuffer();

	void wait();

	~LogicalDevice();

	void queuePresent(SwapChain& swapchain, RenderPass& renderPass, CommandBuffers& commandBuffers, SyncObjects& syncObjects, uint32_t currentFrame, uint32_t imageIndex, bool framebufferResized);
	void queueSubmitForSingleBuffer(CommandBuffer* commandBuffer);
	void queueWaitIdleGraphics();

	GENERATETYPE(LogicalDevice)
private:
	VkDevice device;
	Device* owner;
	CommandPool* commandPool;
	GPUQueue* graphicsQueue;
	GPUQueue* presentQueue;
	GPUQueue* transferQueue;
	GPUQueue* computeQueue;
	FrameBuffers* frameBuffers;
};

#endif