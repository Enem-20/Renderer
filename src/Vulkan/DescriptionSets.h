#pragma once

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Texture2D;
class DescriptorSetLayout;
class DescriptorPool;
class UniformBuffers;
class LogicalDevice;

class DescriptionSets {
public:
	DescriptionSets(LogicalDevice& logicalDevice, DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool, UniformBuffers& uniformBuffers, const std::vector<std::unique_ptr<Texture2D>>& textures);

	std::vector<VkDescriptorSet>& getDescriptorSets();
private:
	DescriptorSetLayout& descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;
};