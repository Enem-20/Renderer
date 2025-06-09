#include "SyncObjects.h"

#include "LogicalDevice.h"
#include "Instance.h"
#include "Device.h"

#include "Resources/ResourceManager.h"

#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>

SyncObjects::SyncObjects(const std::string& name, LogicalDevice* logicalDevice)
	: logicalDevice(logicalDevice)
	, ResourceBase(name)
{
	uint32_t inFlight = logicalDevice->getOwner()->instance->getFramesInFlight();
	imageAvailableSemaphores.resize(inFlight);
	renderFinishedSemaphores.resize(inFlight);
	inFlightFences.resize(inFlight);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < inFlight; ++i) {
		if (vkCreateSemaphore(logicalDevice->getRaw(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS
			|| vkCreateSemaphore(logicalDevice->getRaw(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS
			|| vkCreateFence(logicalDevice->getRaw(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores or fences!");
		}
	}

	ResourceManager::addResource<SyncObjects>(this);
}

SyncObjects::~SyncObjects() {
	for (size_t i = 0; i < logicalDevice->getOwner()->instance->getFramesInFlight(); ++i) {
		vkDestroySemaphore(logicalDevice->getRaw(), renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(logicalDevice->getRaw(), imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(logicalDevice->getRaw(), inFlightFences[i], nullptr);
	}
}

std::vector<VkSemaphore>& SyncObjects::getImageAvailableSemaphores() {
	return imageAvailableSemaphores;
}

std::vector<VkSemaphore>& SyncObjects::getRenderFinishedSemaphores() {
	return renderFinishedSemaphores;
}

std::vector<VkFence>& SyncObjects::getInFlightFences() {
	return inFlightFences;
}

void SyncObjects::waitForFences(uint32_t currentFrame) {
	vkWaitForFences(logicalDevice->getRaw(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
}

void SyncObjects::resetFences(uint32_t currentFrame) {
	vkResetFences(logicalDevice->getRaw(), 1, &inFlightFences[currentFrame]);
}