#include "DescriptorSetBase.h"

#include <GLFW/glfw3.h>

std::vector<VkDescriptorSet> DescriptorSetBase::getDescriptorSets() {
	return descriptorSets;
}