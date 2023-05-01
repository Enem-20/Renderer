#pragma once

#include "Capturer.h"

#include <GLFW/glfw3.h>

#include <optional>

class Instance;

class DebugMessanger{
public:
	DebugMessanger(Instance& instance);
	~DebugMessanger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
private:
	void destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);
	void destroyDebugMessenger();

	VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	VkDebugUtilsMessengerEXT debugMessenger;
	Instance& instance;
};