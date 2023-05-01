#pragma once

#include "GLFW/glfw3.h"

#include <vector>
#include <memory>

class LogicalDevice;

struct SyncObjects {
public:
	SyncObjects(LogicalDevice& logicalDevice);
	~SyncObjects();

	std::vector<VkSemaphore>& getImageAvailableSemaphores();
	std::vector<VkSemaphore>& getRenderFinishedSemaphores();
	std::vector<VkFence>& getInFlightFences();

	void waitForFences(uint32_t currentFrame);
	void resetFences(uint32_t currentFrame);
private:
	LogicalDevice& logicalDevice;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
};