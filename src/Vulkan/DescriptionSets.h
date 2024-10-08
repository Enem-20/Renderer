#pragma once


#ifndef DESCRIPTION_SETS_H
#define DESCRIPTION_SETS_H


#include "Resources/ResourceBase.h"
#include "API/ExportPropety.h"

#include <vector>
#include <memory>

class Texture2D;
class DescriptorSetLayout;
class DescriptorPool;
class UniformBuffers;
class LogicalDevice;

struct VkDescriptorSet_T;
typedef VkDescriptorSet_T* VkDescriptorSet;

class DLLEXPORT DescriptionSets : public ResourceBase{
public:
	DescriptionSets(const std::string& name, LogicalDevice& logicalDevice, DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool, UniformBuffers& uniformBuffers, const std::vector<std::shared_ptr<Texture2D>>& textures);

	std::vector<VkDescriptorSet>& getDescriptorSets();

	GENERATETYPE(DescriptionSets)
private:
	DescriptorSetLayout& descriptorSetLayout;
	std::vector<VkDescriptorSet> descriptorSets;
};

#endif