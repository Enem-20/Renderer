#include "GeneralVulkanStorage.h"

#include "Instance.h"
#include "DebugMessenger.h"

#include <iostream>


std::vector<const char*> GeneralVulkanStorage::validationLayers = {
	   "VK_LAYER_KHRONOS_validation"
};

GeneralVulkanStorage* GeneralVulkanStorage::getInstance() {
	if (instance == nullptr) [[unlikely]] {
		instance = new GeneralVulkanStorage();
	}
	return instance;
}