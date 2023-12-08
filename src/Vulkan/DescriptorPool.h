#pragma once

#include "UniformBuffer.h"

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"


#include <memory>

class LogicalDevice;
class CommandPool;

struct VkDescriptorPool_T;
typedef VkDescriptorPool_T* VkDescriptorPool;

class DLLEXPORT DescriptorPool : public ResourceBase{
public:
	DescriptorPool(const std::string& name, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~DescriptorPool();

	VkDescriptorPool& getRaw();

	GENERATETYPE(DescriptorPool)
private:
	LogicalDevice& logicalDevice;
	CommandPool& commandPool;
	VkDescriptorPool descriptorPool;
};