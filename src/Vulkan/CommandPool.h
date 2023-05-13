#pragma once

#include "../../src/Resources/ResourceBase.h"

#include <GLFW\glfw3.h>

class PhysicalDevice;
class LogicalDevice;

#include <memory>

class CommandPool : public ResourceBase{
public:
	CommandPool(const std::string& name, PhysicalDevice& pPhysicalDevice, LogicalDevice& pLogicalDevice);
	VkCommandPool& getRaw();
	~CommandPool();

	inline static const std::string type = GETTYPE(CommandPool);
private:
	VkCommandPool commandPool;

	//PhysicalDevice& pPhysicalDevice;
	LogicalDevice& pLogicalDevice;
};