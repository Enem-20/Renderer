#pragma once

#include "../../src/Resources/ResourceBase.h"
#include "../../src/ExportPropety.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Texture2D;
class DescriptorSetLayout;
class DescriptorPool;
class UniformBuffers;
class LogicalDevice;

class DLLEXPORT DescriptionSets : public ResourceBase{
public:
	DescriptionSets(const std::string& name, LogicalDevice& logicalDevice, DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool, UniformBuffers& uniformBuffers, const std::vector<std::shared_ptr<Texture2D>>& textures);

	std::vector<VkDescriptorSet>& getDescriptorSets();

	GENERATETYPE(DescriptionSets)
private:
	DescriptorSetLayout& descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;
};