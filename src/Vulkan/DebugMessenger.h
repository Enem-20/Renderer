#pragma once

#include "../../src/Resources/ResourceBase.h"

#include <optional>

#if defined(_WIN32)
#define VKAPI_ATTR
#define VKAPI_CALL __stdcall
#endif

class Instance;

struct VkDebugUtilsMessengerCreateInfoEXT;
struct VkDebugUtilsMessengerCallbackDataEXT;
struct VkAllocationCallbacks;
struct VkDebugUtilsMessengerEXT_T;
typedef VkDebugUtilsMessengerEXT_T* VkDebugUtilsMessengerEXT;

typedef uint32_t VkFlags;
typedef VkFlags VkDebugUtilsMessageTypeFlagsEXT;
typedef uint32_t VkBool32;

enum VkDebugUtilsMessageSeverityFlagBitsEXT;
enum VkResult;

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

	GENERATETYPE(DebugMessenger)
private:
	void destroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);
	void destroyDebugMessenger();

	VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	VkDebugUtilsMessengerEXT debugMessenger;
	Instance& instance;
};