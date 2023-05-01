#include "CommandPool.h"

#include "PhysicalDevice.h"
#include "LogicalDevice.h"

CommandPool::CommandPool(PhysicalDevice& pPhysicalDevice, LogicalDevice& pLogicalDevice)
	: pLogicalDevice(pLogicalDevice)
{
	QueueFamilyIndices queueFamilyIndices = pPhysicalDevice.findQueueFamiliesThisDevice();

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(pLogicalDevice.getRaw(), &poolInfo, nullptr, &commandPool)) {
		throw std::runtime_error("failed to create command pool!");
	}
}

CommandPool::~CommandPool() {
	//pLogicalDevice.wait();
	vkDestroyCommandPool(pLogicalDevice.getRaw(), commandPool, nullptr);
}

VkCommandPool& CommandPool::getRaw() {
	return commandPool;
}