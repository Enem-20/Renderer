#pragma once

#include "../../src/Resources/ResourceBase.h"

#include <GLFW/glfw3.h>

#include <optional>

class Instance;

class DebugMessenger : public ResourceBase{
public:
	DebugMessenger(const std::string& name, Instance& instance);
	DebugMessenger(const DebugMessenger& debugMessenger);
	~DebugMessenger();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	inline static const std::string type = GETTYPE(DebugMessenger);
private:
	void destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);
	void destroyDebugMessenger();

	VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	VkDebugUtilsMessengerEXT debugMessenger;
	Instance& instance;
};