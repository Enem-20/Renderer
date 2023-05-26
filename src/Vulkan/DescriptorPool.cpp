#include "DescriptorPool.h"

#include "UniformBuffer.h"
#include "CommandPool.h"
#include "GeneralVulkanStorage.h"
#include "LogicalDevice.h"

#include "../../src/Resources/ResourceManager.h"

#include "GLFW/glfw3.h"


#include <array>
#include <iostream>

DescriptorPool::DescriptorPool(const std::string& name, LogicalDevice& logicalDevice, CommandPool& commandPool)
	: logicalDevice(logicalDevice)
	, commandPool(commandPool)
	, ResourceBase(name)
{
	auto uniformBuffers = ResourceManager::getResourcesWithType<UniformBuffers>();
	auto textures = ResourceManager::getResourcesWithType<Texture2D>();


	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT) * 1000;// * 2;//static_cast<uint32_t>(uniformBuffers->size())
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT) * 1000;// * 2;//static_cast<uint32_t>(textures->size())

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT)* (2000);
	poolInfo.flags = 0;

	if (vkCreateDescriptorPool(logicalDevice.getRaw(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

	ResourceManager::addResource<DescriptorPool>(this);
}

DescriptorPool::~DescriptorPool() {
	//ResourceManager::removeResource<DescriptorPool>(name);
	vkDestroyDescriptorPool(logicalDevice.getRaw(), descriptorPool, nullptr);

	Texture2D::destroyDescriptorSetLayout(logicalDevice);
	UniformBuffers::destroyDescriptorSetLayout(logicalDevice);
}

VkDescriptorPool& DescriptorPool::getRaw() {
	return descriptorPool;
}