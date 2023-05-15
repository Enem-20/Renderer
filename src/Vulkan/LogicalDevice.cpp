#include "LogicalDevice.h"

#include "SyncObjects.h"
#include "CommandPool.h"
#include "SingleTimeBuffer.h"
#include "SwapChain.h"
#include "PhysicalDevice.h"
#include "WindowSurface.h"
#include "CommandBuffer.h"

#include "GeneralVulkanStorage.h"

#include "../../src/Resources/ResourceManager.h"

#include <set>
#include <array>

LogicalDevice::LogicalDevice(const std::string& name, WindowSurface& windowSurface, PhysicalDevice& physicalDevice)
	: physicalDevice(physicalDevice)
	, ResourceBase(name)
{
	QueueFamilyIndices indices = physicalDevice.findQueueFamiliesThisDevice();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

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

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (GeneralVulkanStorage::enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(GeneralVulkanStorage::validationLayers.size());
		createInfo.ppEnabledLayerNames = GeneralVulkanStorage::validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice.getRaw(), &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

	ResourceManager::addResource<LogicalDevice>(this);
}

LogicalDevice::LogicalDevice(const LogicalDevice& logicalDevice)
	: physicalDevice(logicalDevice.physicalDevice)
	, device(logicalDevice.device)
	, graphicsQueue(logicalDevice.graphicsQueue)
	, presentQueue(logicalDevice.presentQueue)
	, ResourceBase(name)
{
	ResourceManager::addResource<LogicalDevice>(this);
}

LogicalDevice::~LogicalDevice() {
	ResourceManager::removeResource<LogicalDevice>(name);
	vkDestroyDevice(device, nullptr);
}

VkDevice& LogicalDevice::getRaw() {
	return device;
}

VkQueue& LogicalDevice::getGraphicsQueue() {
	return graphicsQueue;
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
	allocInfo.memoryTypeIndex = physicalDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void LogicalDevice::wait() {
	vkQueueWaitIdle(presentQueue);
	vkQueueWaitIdle(graphicsQueue);
	vkDeviceWaitIdle(device);
}

void LogicalDevice::queuePresent(SwapChain& swapchain, RenderPipeline& renderPipeline, CommandBuffers& commandBuffers, CommandPool& commandPool, SyncObjects& syncObjects, uint32_t currentFrame, uint32_t imageIndex, bool framebufferResized) {
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncObjects.getImageAvailableSemaphores()[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers.getRaw()[currentFrame];

	VkSemaphore signalSemaphores[] = { syncObjects.getRenderFinishedSemaphores()[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, syncObjects.getInFlightFences()[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain.getRaw()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		swapchain.recreateSwapChain(renderPipeline, commandPool);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}

void LogicalDevice::queueSubmitForSingleBuffer(CommandBuffer& commandBuffer) {
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer.getRaw();

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
}

void LogicalDevice::queueWaitIdleGraphics() {
	vkQueueWaitIdle(graphicsQueue);
}

void LogicalDevice::copyBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	SingleTimeBuffer singleTimeBuffer(logicalDevice, commandPool);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(singleTimeBuffer.getCommandBuffer().getRaw(), srcBuffer, dstBuffer, 1, &copyRegion);
}