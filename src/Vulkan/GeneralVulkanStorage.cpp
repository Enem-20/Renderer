#include "GeneralVulkanStorage.h"

#include "Instance.h"
#include "DebugMessenger.h"

#include <iostream>

std::shared_ptr<Instance> GeneralVulkanStorage::instance;
std::shared_ptr<DebugMessanger> GeneralVulkanStorage::debugMessenger;


std::vector<const char*> GeneralVulkanStorage::validationLayers = {
	   "VK_LAYER_KHRONOS_validation"
};