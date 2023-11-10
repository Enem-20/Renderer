#pragma once

#include "../../src/Resources/ResourceBase.h"

#include <memory>

class LogicalDevice;

struct VkDescriptorSetLayout_T;
typedef VkDescriptorSetLayout_T* VkDescriptorSetLayout;

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