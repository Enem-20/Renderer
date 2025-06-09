#pragma once

#ifndef GENERAL_VULKAN_STORAGE_H
#define GENERAL_VULKAN_STORAGE_H

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Instance;
class DebugMessenger;

class GeneralVulkanStorage {
public:
	GeneralVulkanStorage* getInstance();

	#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif
private:
	GeneralVulkanStorage() = default;
	GeneralVulkanStorage(const GeneralVulkanStorage&) = delete;
	GeneralVulkanStorage(GeneralVulkanStorage&&) = delete;
	GeneralVulkanStorage& operator=(GeneralVulkanStorage&&) = delete;



	static GeneralVulkanStorage* instance;

	static std::vector<const char*> validationLayers;
};

#endif