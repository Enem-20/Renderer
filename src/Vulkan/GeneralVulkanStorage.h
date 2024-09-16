#pragma once

#ifndef GENERAL_VULKAN_STORAGE_H
#define GENERAL_VULKAN_STORAGE_H

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Instance;
class DebugMessenger;

struct GeneralVulkanStorage {
	GeneralVulkanStorage() = delete;
	GeneralVulkanStorage(const GeneralVulkanStorage&) = delete;
	GeneralVulkanStorage(GeneralVulkanStorage&&) = delete;
	GeneralVulkanStorage& operator=(GeneralVulkanStorage&&) = delete;

	static std::shared_ptr<Instance> instance;
	static std::shared_ptr<DebugMessenger> debugMessenger;

#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif

	static std::vector<const char*> validationLayers;
	static const int MAX_FRAMES_IN_FLIGHT = 2;
};

#endif