#pragma once

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class LogicalDevice;
class SwapChain;

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

class UniformBuffers {
public:
	UniformBuffers(LogicalDevice& logicalDevice, SwapChain& swapchain);
	~UniformBuffers();

	std::vector<VkBuffer>& getRaw();

	void updateUniformBuffer(uint32_t currentImage);
private:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	LogicalDevice& logicalDevice;
	SwapChain& swapchain;
};