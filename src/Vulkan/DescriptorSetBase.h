#pragma once

#ifndef DESCRIPTORSETBASE
#define DESCRIPTORSETBASE

#include "../../src/ExportPropety.h"

#include <optional>
#include <memory>
#include <vector>

struct VkDescriptorSet_T;
typedef VkDescriptorSet_T* VkDescriptorSet;

class DescriptorPool;

class DLLEXPORT DescriptorSetBase {
public:
	DescriptorSetBase() = default;

	std::vector<VkDescriptorSet> getDescriptorSets();

	virtual void createDescriptorSets(DescriptorPool& descriptorPool) = 0;
protected:
	std::vector<VkDescriptorSet> descriptorSets;
};
#endif // !DESCRIPTORSETBASE
