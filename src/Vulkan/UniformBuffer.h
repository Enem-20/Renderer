#pragma once

#include "../../src/Resources/ResourceBase.h"

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class LogicalDevice;
class SwapChain;

class UniformBuffers : public ResourceBase{
public:
	UniformBuffers(const std::string& name, LogicalDevice& logicalDevice, SwapChain& swapchain);
	~UniformBuffers();

	std::vector<VkBuffer>& getRaw();

	void updateUniformBuffer(uint32_t currentImage);
	inline static const std::string type = GETTYPE(UniformBuffers);
private:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	LogicalDevice& logicalDevice;
	SwapChain& swapchain;
};