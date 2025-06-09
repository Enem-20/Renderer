#include "CommandPool.h"

#include "Resources/ResourceManager.h"

#include "Device.h"

#include <GLFW\glfw3.h>

CommandPool::CommandPool(const std::string& name, Device* device)
	: ResourceBase(name)
	, virtualDevice(device)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = device->device.getGraphicsIndices()[0];

	if (vkCreateCommandPool(virtualDevice->logicalDevice.getRaw(), &poolInfo, nullptr, &commandPool)) {
		throw std::runtime_error("failed to create command pool!");
	}

	ResourceManager::addResource<CommandPool>(this);
}

CommandPool::~CommandPool() {
	vkDestroyCommandPool(virtualDevice->logicalDevice.getRaw(), commandPool, nullptr);
}

VkCommandPool& CommandPool::getRaw() {
	return commandPool;
}