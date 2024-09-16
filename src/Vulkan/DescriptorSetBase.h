#pragma once

#ifndef DESCRIPTOR_SET_BASE_H
#define DESCRIPTOR_SET_BASE_H

#include "API/ExportPropety.h"

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
