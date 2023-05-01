#pragma once

#include "GLFW/glfw3.h"

#include <vector>
#include <memory>

class LogicalDevice;
class CommandPool;

class IndexBuffer {
public:
	IndexBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool);
	~IndexBuffer();

	VkBuffer& getRaw();
	const std::vector<uint16_t>& getIndices();
private:
	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
	};

	LogicalDevice& logicalDevice;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
};