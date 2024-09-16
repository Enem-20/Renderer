#include "DebugMessenger.h"

#include "Resources/ResourceManager.h"
#include "GeneralVulkanStorage.h"

#include "Instance.h"

#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>
#include <memory>

DebugMessenger::DebugMessenger(const std::string& name, Instance& instance)
	: instance(instance)
	, ResourceBase(name)
{
	if (!GeneralVulkanStorage::enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (DebugMessenger::CreateDebugUtilsMessengerEXT(&createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}

	ResourceManager::addResource<DebugMessenger>(this);
}

DebugMessenger::~DebugMessenger() {
	destroyDebugMessenger();
}

DebugMessenger::DebugMessenger(const DebugMessenger& debugMessenger) 
	: debugMessenger(debugMessenger.debugMessenger)
	, instance(debugMessenger.instance)
	, ResourceBase(debugMessenger.name)
{

}

void DebugMessenger::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;
}

VkResult DebugMessenger::CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.getRaw(), "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance.getRaw(), pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DebugMessenger::destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.getRaw(), "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance.getRaw(), debugMessenger, pAllocator);
	}
}

void DebugMessenger::destroyDebugMessenger() {
	if (GeneralVulkanStorage::enableValidationLayers) {
		destroyDebugUtilsMessengerEXT(nullptr);
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}