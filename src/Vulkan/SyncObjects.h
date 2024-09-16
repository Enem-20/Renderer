#pragma once

#ifndef SYNC_OBJECTS_H
#define SYNC_OBJECTS_H

#include "Resources/ResourceBase.h"

#include <vector>
#include <memory>

struct VkSemaphore_T;
typedef VkSemaphore_T* VkSemaphore;
struct VkFence_T;
typedef VkFence_T* VkFence;

class LogicalDevice;

struct SyncObjects : public ResourceBase{
public:
	SyncObjects(const std::string& name, LogicalDevice& logicalDevice);
	~SyncObjects();

	std::vector<VkSemaphore>& getImageAvailableSemaphores();
	std::vector<VkSemaphore>& getRenderFinishedSemaphores();
	std::vector<VkFence>& getInFlightFences();

	void waitForFences(uint32_t currentFrame);
	void resetFences(uint32_t currentFrame);

	GENERATETYPE(SyncObjects)
private:
	LogicalDevice& logicalDevice;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
};

#endif