#pragma once

#include "../Texture2D.h"
#include "UniformBuffer.h"

#include "../../../src/Resources/ResourceBase.h"
#include "../../../src/ExportPropety.h"

#include <glfw/glfw3.h>

#include <memory>

class LogicalDevice;
class CommandPool;


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