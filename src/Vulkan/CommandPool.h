#pragma once

#include <GLFW\glfw3.h>

class PhysicalDevice;
class LogicalDevice;

#include <memory>

class CommandPool {
public:
	CommandPool(PhysicalDevice& pPhysicalDevice, LogicalDevice& pLogicalDevice);
	VkCommandPool& getRaw();
	~CommandPool();
private:
	VkCommandPool commandPool;

	//PhysicalDevice& pPhysicalDevice;
	LogicalDevice& pLogicalDevice;
};