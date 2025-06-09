#include "Instance.h"

#include <cstddef>
#include <string>

#include "DebugMessenger.h"
#include "GeneralVulkanStorage.h"
#include "Device.h"
#include "Frame.h"
#include "PhysicalDevice.h"
#include "PhysicalDeviceSelector.h"
#include "WindowSurface.h"
#include "RenderPass.h"




Instance::Instance(const std::string& name, std::vector<const char*> validationLayers,  VulkanObject* parent)
	: ResourceBase(name)
	, VulkanObject(parent)
	, maxFramesInFlight(2)
	, validationLayers(validationLayers)
	, framesInFlight(maxFramesInFlight)
	
{
	for(size_t i = 0; i < maxFramesInFlight; ++i) {
		framesInFlight[i] = new Frame(i);
	}
	if (GeneralVulkanStorage::enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto glfwExtensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();

	if (GeneralVulkanStorage::enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	//Getting available extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);//Extensions details.
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:\n";

	for (const auto& extension : extensions) {
		std::cout << '\t' << extension.extensionName << '\n';
	}

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (GeneralVulkanStorage::enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
	mainWindowSurface = new WindowSurface("mainWindow", this);
	windowSurfaces.emplace(mainWindowSurface->name, mainWindowSurface);
	PhysicalDeviceSelector::getInstance()->collectRenderDevices(this, mainWindowSurface);
	
}

std::vector<const char*> Instance::getRequiredExtensions() const {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (GeneralVulkanStorage::enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool Instance::checkValidationLayerSupport() const{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

Instance::~Instance() {
	vkDestroyInstance(instance, nullptr);
}

VkInstance& Instance::getRaw() {
	return instance;
}

void Instance::addDevice(std::string_view name, Device* device) {
	if(!devices.contains(name.data())) {
		devices.emplace(name.data(), device);
		std::cout << "device " << name << " added" << '\n';
	}
	else {
		std::cerr << "Error: this device already exists" << '\n';
	}
}

void Instance::removeDevice(std::string_view name) {
	if(devices.find(name.data()) != devices.end()) {
		delete devices[name.data()];
		devices.erase(name.data());
	}
	else {
		std::cerr << "Error: this device doesn't exist" << '\n';
	}
}

void Instance::addRenderPass(std::string_view name, RenderPass* renderPass) {
	if(!renderPasses.contains(name.data())) {
		renderPasses.emplace(name.data(), renderPass);
		std::cout << "renderpass " << name << " added" << '\n';
	}
	else {
		std::cerr << "Error: this renderpass already exists" << '\n';
	}
}

WindowSurface* Instance::getWindowSurface(std::string_view name) {
	return windowSurfaces[name.data()];
}

uint32_t Instance::getMaxFramesInFlight() const {
	return maxFramesInFlight;
}

void Instance::setMaxFramesInFlight(uint32_t maxFramesInFlight) { this->maxFramesInFlight = maxFramesInFlight; }

std::vector<Frame*>& Instance::getFramesInFlight() {
	return framesInFlight;
}

uint32_t Instance::getCurrentFrame() const {
	return currentFrame;
}

const std::vector<const char*>& Instance::getValidationLayers() const {
	return validationLayers;
}

void Instance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugMessenger::debugCallback;
	createInfo.pUserData = nullptr;
}