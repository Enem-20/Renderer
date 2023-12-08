#pragma once

#include "Resources/ResourceBase.h"

class PhysicalDevice;
class LogicalDevice;

struct VkCommandPool_T;
typedef VkCommandPool_T* VkCommandPool;

#include <memory>

class CommandPool : public ResourceBase{
public:
	CommandPool(const std::string& name, PhysicalDevice& pPhysicalDevice, LogicalDevice& pLogicalDevice);
	VkCommandPool& getRaw();
	~CommandPool();

	GENERATETYPE(CommandPool)
private:
	VkCommandPool commandPool;

	LogicalDevice& pLogicalDevice;
};