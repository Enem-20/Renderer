#pragma once

#include "Resources/ResourceBase.h"
#include "API/ExportPropety.h"

#include <vector>
#include <memory>


class DescriptorSetLayout;
class DescriptorPool;
class UniformBuffers;
class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class VulkanTexture2D;

struct VkDescriptorSet_T;
typedef VkDescriptorSet_T* VkDescriptorSet;

class DLLEXPORT DescriptionSets : public ResourceBase{
public:
	DescriptionSets(std::string_view name, LogicalDevice& logicalDevice, DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool, UniformBuffers& uniformBuffers, const std::vector<std::shared_ptr<VulkanTexture2D>>& textures);

	std::vector<VkDescriptorSet>& getDescriptorSets();

	GENERATETYPE(DescriptionSets)
private:
	DescriptorSetLayout& descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;
};