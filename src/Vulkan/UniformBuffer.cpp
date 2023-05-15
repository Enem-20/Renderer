#include "UniformBuffer.h"

#include "../../../src/GameTypes/GameObject.h"
#include "../Sprite.h"
#include "../../src/Resources/ResourceManager.h"

#include "SwapChain.h"
#include "GeneralVulkanStorage.h"
#include "LogicalDevice.h"
#include "../UniformBufferObject.h"


#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

UniformBuffers::UniformBuffers(const std::string& name, LogicalDevice& logicalDevice, SwapChain& swapchain)
	: logicalDevice(logicalDevice)
	, swapchain(swapchain)
	, ResourceBase(name)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT; ++i) {
		logicalDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(logicalDevice.getRaw(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}

	ResourceManager::addResource<UniformBuffers>(this);
}

UniformBuffers::~UniformBuffers() {
	ResourceManager::removeResource<UniformBuffers>(name);
	for (size_t i = 0; i < GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroyBuffer(logicalDevice.getRaw(), uniformBuffers[i], nullptr);
		vkFreeMemory(logicalDevice.getRaw(), uniformBuffersMemory[i], nullptr);
	}
}

std::vector<VkBuffer>& UniformBuffers::getRaw() {
	return uniformBuffers;
}

void UniformBuffers::updateUniformBuffer(uint32_t currentImage) {	
	auto spritesToRender = ResourceManager::getResourcesWithType<Sprite>();
	for (auto& [key, value] : *spritesToRender) {
		auto ubo = value.getResource<Sprite>()->getUBO();
		ubo.model = glm::mat4(1.0f);
		ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		auto width = swapchain.getSwapchainExtent().width;
		auto height = swapchain.getSwapchainExtent().height;
		ubo.proj = glm::perspective(glm::radians(90.0f), swapchain.getSwapchainExtent().width / (float)swapchain.getSwapchainExtent().height, 0.1f, 100.0f);
		//ubo.proj = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 0.1f, 1000.f);

		ubo.proj[1][1] *= -1;
		memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
	}

	//auto gameObjects = ResourceManager::getResourcesWithType<GameObject>();
	//for (auto& [key, value] : *gameObjects) {
	//	value.getResource<GameObject>()->Update();
	//}
}