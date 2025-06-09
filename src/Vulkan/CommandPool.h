#pragma once

#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include "Resources/ResourceBase.h"

class Device;

struct VkCommandPool_T;
typedef VkCommandPool_T* VkCommandPool;

class CommandPool : public ResourceBase{
public:
	CommandPool(const std::string& name, Device* device);
	VkCommandPool& getRaw();
	~CommandPool();

	GENERATETYPE(CommandPool)
private:
	VkCommandPool commandPool;

	Device* virtualDevice;
};

#endif