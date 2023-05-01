#pragma once

#include <GLFW/glfw3.h>

#include <memory>

class LogicalDevice;

class DescriptorSetLayout {
public:
	DescriptorSetLayout(LogicalDevice& logicalDevice);
	~DescriptorSetLayout();

	VkDescriptorSetLayout& getDescriptorSetLayout();
private:
	LogicalDevice& logicalDevice;

	VkDescriptorSetLayout descriptorSetLayout;
};