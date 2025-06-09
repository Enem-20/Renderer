#include "LogicalDevice.h"



#include <GLFW/glfw3.h>

#include <cstdint>
#include <memory>
#include <set>
#include <array>



#include "FrameBuffers.h"
#include "GPUQueue.h"
#include "WindowSurface.h"
#include "SyncObjects.h"
#include "CommandPool.h"
#include "SingleTimeBuffer.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "PhysicalDevice.h"
#include "WindowSurface.h"
#include "CommandBuffer.h"
#include "Device.h"

#include "GeneralVulkanStorage.h"

LogicalDevice::LogicalDevice(const std::string& name, Device* owner)
	: ResourceBase(name)
	, owner(owner)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	uint32_t graphicsFamilyIndex = owner->device.getGraphicsIndices()[0];
	uint32_t presentFamilyIndex = owner->device.getPresentIndices()[0];
	
	std::set<uint32_t> uniqueQueueFamilies = { graphicsFamilyIndex, presentFamilyIndex };

	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;

	auto supportedDeviceExtensions = owner->device.getDeviceExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(supportedDeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = supportedDeviceExtensions.data();

	if (GeneralVulkanStorage::enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(owner->instance->getValidationLayers().size());
		createInfo.ppEnabledLayerNames = owner->instance->getValidationLayers().data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(owner->device.getRaw(), &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device");
	}

	graphicsQueue = new GPUQueue(this, graphicsFamilyIndex, owner->instance->getMaxFramesInFlight());
	presentQueue = new GPUQueue(this, presentFamilyIndex, owner->instance->getMaxFramesInFlight());
}

LogicalDevice::LogicalDevice(const LogicalDevice& logicalDevice)
	: ResourceBase(logicalDevice.name)
	, device(logicalDevice.device)
	, owner(logicalDevice.owner)
	, graphicsQueue(logicalDevice.graphicsQueue)
	, presentQueue(logicalDevice.presentQueue)
{

}

LogicalDevice::~LogicalDevice() {
	vkDestroyDevice(device, nullptr);
}

VkDevice LogicalDevice::getRaw() const{
	return device;
}

Device* LogicalDevice::getOwner() const {
	return owner;
}

GPUQueue* LogicalDevice::getGraphicsQueue() const {
	return graphicsQueue;
}

CommandPool* LogicalDevice::getCommandPool() const {
	return commandPool;
}

void LogicalDevice::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties
	, VkBuffer& buffer, VkDeviceMemory& bufferMemory) 
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;

	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = owner->device.findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void LogicalDevice::wait() {
	presentQueue->waitIdle();
	graphicsQueue->waitIdle();
	vkDeviceWaitIdle(device);
}

void LogicalDevice::queuePresent(SwapChain& swapchain, RenderPass& renderPass, CommandBuffers& commandBuffers, SyncObjects& syncObjects, uint32_t currentFrame, uint32_t imageIndex, bool framebufferResized) {
	

	
}

void LogicalDevice::queueSubmitForSingleBuffer(CommandBuffer* commandBuffer) {
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer->getRaw();

	vkQueueSubmit(graphicsQueue->getRaw(), 1, &submitInfo, VK_NULL_HANDLE);
}

void LogicalDevice::queueWaitIdleGraphics() {
	graphicsQueue->waitIdle();
}

void LogicalDevice::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	SingleTimeBuffer singleTimeBuffer(this, commandPool);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(singleTimeBuffer.getCommandBuffer()->getRaw(), srcBuffer, dstBuffer, 1, &copyRegion);
}

VkFence LogicalDevice::createFence() const {
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkFence fence;
	if (vkCreateFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create fence!");
	}

	return fence;
}

VkSemaphore LogicalDevice::createSemaphore() const {
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore semaphore;
	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
		throw std::runtime_error("failed to create semaphore!");
	}

	return semaphore;
}

void LogicalDevice::createFrameBuffers() {
	frameBuffers = new FrameBuffers(owner->instance->getMaxFramesInFlight(), owner->instance->getRenderPass(), owner->instance->getMainWindowSurface()->getImageViews());
}

SingleTimeBuffer* LogicalDevice::createSingleTimeBuffer() {

}