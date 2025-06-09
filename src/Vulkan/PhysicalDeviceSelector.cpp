#include "PhysicalDeviceSelector.h"
#include <cstddef>
#include <set>

#include <GLFW/glfw3.h>
#include <string>

#include "Instance.h"
#include "PhysicalDevice.h"
#include "Device.h"

PhysicalDeviceSelector* PhysicalDeviceSelector::selectorInstance = nullptr;
const std::vector<const char*> PhysicalDeviceSelector::deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

PhysicalDeviceSelector* PhysicalDeviceSelector::getInstance() {
    if(selectorInstance == nullptr) [[unlikely]]{
        return selectorInstance = new PhysicalDeviceSelector();
    }
    return nullptr;
}

void PhysicalDeviceSelector::collectRenderDevices(Instance* instance, WindowSurface* surface) {
    uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->getRaw(), &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->getRaw(), &deviceCount, devices.data());

	size_t i = 0;
	for (const auto& device : devices) {
		auto virtualDevice = new Device("Device", instance, device);
		instance->addDevice("d" + std::to_string(i), virtualDevice);
		if (isDeviceSuitable(&(virtualDevice->device), surface)) {
			surface->addDevice("d" + std::to_string(i), virtualDevice);
			getMaxUsableSampleCount(&(virtualDevice->device));
			break;
		}
	}
}

bool PhysicalDeviceSelector::checkDeviceExtensionSupport(PhysicalDevice* device) const {
    uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device->getRaw(), nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device->getRaw(), nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());


	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}


bool PhysicalDeviceSelector::isDeviceSuitable(PhysicalDevice* device, WindowSurface* windowSurface) const {
    device->findQueueFamilies(windowSurface);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChain::SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, windowSurface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device->getRaw(), &deviceProperties);

	std::cout << "DeviceName: " << deviceProperties.deviceName << std::endl;

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device->getRaw(), &supportedFeatures);

	return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && device->isRenderSupport() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

size_t PhysicalDeviceSelector::getMaxUsableSampleCount(PhysicalDevice* device) const {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device->getRaw(), &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return  VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return  VK_SAMPLE_COUNT_2_BIT; }

	return  VK_SAMPLE_COUNT_1_BIT;
}