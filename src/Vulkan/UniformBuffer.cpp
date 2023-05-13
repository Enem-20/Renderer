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
	//static auto startTime = std::chrono::high_resolution_clock::now();

	//auto currentTime = std::chrono::high_resolution_clock::now();
	//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	//UniformBufferObject ubo{};
	//ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.proj = glm::perspective(glm::radians(45.0f), swapchain.getSwapchainExtent().width / (float)swapchain.getSwapchainExtent().height, 0.1f, 10.0f);

	//ubo.proj[1][1] *= -1;
	auto spritesToRender = ResourceManager::getResourcesWithType<Sprite>();
	for (auto& [key, value] : *spritesToRender) {
		auto ubo = value.getResource<Sprite>()->getUBO();
		memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
	}

	auto gameObjects = ResourceManager::getResourcesWithType<GameObject>();
	for (auto& [key, value] : *gameObjects) {
		value.getResource<GameObject>()->Update();
	}
}