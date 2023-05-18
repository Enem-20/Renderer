#pragma once

#ifndef DESCRIPTORSETBASE
#define DESCRIPTORSETBASE

//#include "DescriptorPool.h"

#include <GLFW/glfw3.h>

#include <optional>
#include <memory>
#include <vector>

class DescriptorPool;

class DescriptorSetBase {
public:
	DescriptorSetBase() = default;

	std::vector<VkDescriptorSet> getDescriptorSets();

	virtual void createDescriptorSets(DescriptorPool& descriptorPool) = 0;
protected:

	//virtual void createDescriptorSetLayout() = 0;

	std::vector<VkDescriptorSet> descriptorSets;
};
#endif // !DESCRIPTORSETBASE
