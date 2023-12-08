#include "PhysicalDevice.h"

#include "WindowSurface.h"

#include "Resources/ResourceManager.h"

#include <GLFW/glfw3.h>

#include <set>

PhysicalDevice::PhysicalDevice(const std::string& name, Instance& instance, WindowSurface& windowSurface)
	: instance(instance)
	, windowSurface(windowSurface)
	, ResourceBase(name)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.getRaw(), &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance.getRaw(), &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			this->device = device;
			getMaxUsableSampleCount();
			break;
		}
	}

	if (device == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU");
	}



	ResourceManager::addResource<PhysicalDevice>(this);
}

bool PhysicalDevice::isThisDeviceSuitable(){
	QueueFamilyIndices indices = findQueueFamiliesThisDevice();

	bool extensionsSupported = checkThisDeviceExtensionSupport();

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChain::SwapChainSupportDetails swapChainSupport = querySwapChainSupportThisDevice();
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			
	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	std::cout << "DeviceName: " << deviceProperties.deviceName << std::endl;

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return /*(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && */ indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

std::shared_ptr<VkSampleCountFlagBits> PhysicalDevice::getMaxUsableSampleCount() {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return std::make_shared<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_64_BIT); }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return std::make_shared<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_32_BIT); }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return std::make_shared<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_16_BIT); }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return std::make_shared<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_8_BIT); }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return  std::make_shared<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_4_BIT); }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return  std::make_shared<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_2_BIT); }

	return  std::make_shared<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_1_BIT);
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices indices = *findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChain::SwapChainSupportDetails swapChainSupport = *querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	std::cout << "DeviceName: " << deviceProperties.deviceName << std::endl;

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return /*(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && */ indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

std::shared_ptr<VkSampleCountFlagBits> PhysicalDevice::getMsaaSamples() {
	return getMaxUsableSampleCount();
}

bool PhysicalDevice::checkThisDeviceExtensionSupport() const{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());


	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) const {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());


	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices PhysicalDevice::findQueueFamiliesThisDevice() {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		//if (queueFamily.queueCount & VK_QUEUE_GRAPHICS_BIT) {
		indices.graphicsFamily = i;
		//}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowSurface.getRaw(), &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) { break; }

		i++;
	}

	return std::move(indices);
}

std::shared_ptr<QueueFamilyIndices> PhysicalDevice::findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		//if (queueFamily.queueCount & VK_QUEUE_GRAPHICS_BIT) {
		indices.graphicsFamily = i;
		//}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowSurface.getRaw(), &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) { break; }

		i++;
	}

	return std::make_shared<QueueFamilyIndices>(indices);
}

SwapChain::SwapChainSupportDetails PhysicalDevice::querySwapChainSupportThisDevice() const{
	SwapChain::SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, windowSurface.getRaw(), details.capabilities.get());

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface.getRaw(), &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface.getRaw(), &formatCount,details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface.getRaw(), &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface.getRaw(), &presentModeCount, details.presentModes.data());
	}

	return details;
}

std::shared_ptr<SwapChain::SwapChainSupportDetails> PhysicalDevice::querySwapChainSupport(VkPhysicalDevice device) const {
	SwapChain::SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, windowSurface.getRaw(), details.capabilities.get());

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface.getRaw(), &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowSurface.getRaw(), &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface.getRaw(), &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowSurface.getRaw(), &presentModeCount, details.presentModes.data());
	}

	return std::make_shared<SwapChain::SwapChainSupportDetails>(details);
}

uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags& properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

VkPhysicalDevice& PhysicalDevice::getRaw() {
	return device;
}

std::shared_ptr<VkFormat> PhysicalDevice::findSupportedFormat(const std::vector < std::shared_ptr<VkFormat>>& candidates, std::shared_ptr<VkImageTiling> tiling, VkFormatFeatureFlags features) {
	for (std::shared_ptr<VkFormat> format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(device, *format, &props);

		if (*tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (*tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

std::shared_ptr<VkFormat> PhysicalDevice::findDepthFormat()
{
	return findSupportedFormat({ std::make_shared<VkFormat>(VK_FORMAT_D32_SFLOAT), std::make_shared<VkFormat>(VK_FORMAT_D32_SFLOAT_S8_UINT), std::make_shared<VkFormat>(VK_FORMAT_D24_UNORM_S8_UINT)},
		std::make_shared<VkImageTiling>(VK_IMAGE_TILING_OPTIMAL),
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}