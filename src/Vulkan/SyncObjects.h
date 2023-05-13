#pragma once

#include "../../src/Resources/ResourceBase.h"

#include "GLFW/glfw3.h"

#include <vector>
#include <memory>

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

	inline static const std::string type = GETTYPE(SyncObjects);
private:
	LogicalDevice& logicalDevice;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
};