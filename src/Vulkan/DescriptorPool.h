#pragma once

#include <glfw/glfw3.h>

#include <memory>

class LogicalDevice;
class CommandPool;


class DescriptorPool {
public:
	DescriptorPool(LogicalDevice& logicalDevice, CommandPool& commandPool);
	~DescriptorPool();

	VkDescriptorPool& getRaw();
private:
	LogicalDevice& logicalDevice;
	CommandPool& commandPool;
	VkDescriptorPool descriptorPool;
};