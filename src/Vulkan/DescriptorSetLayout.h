#pragma once

#include "../../src/Resources/ResourceBase.h"

#include <GLFW/glfw3.h>

#include <memory>

class LogicalDevice;

class DescriptorSetLayout : public ResourceBase{
public:
	DescriptorSetLayout(const std::string& name, LogicalDevice& logicalDevice);
	~DescriptorSetLayout();

	VkDescriptorSetLayout& getDescriptorSetLayout();

	GENERATETYPE(DescriptorSetLayout)
private:
	LogicalDevice& logicalDevice;

	VkDescriptorSetLayout descriptorSetLayout;
};