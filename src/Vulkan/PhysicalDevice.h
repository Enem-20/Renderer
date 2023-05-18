#pragma once

#include "Capturer.h"
#include "Instance.h"
#include "SwapChain.h"

#include "../../src/ExportPropety.h"

#include <GLFW/glfw3.h>

#include <optional>
#include <memory>

class WindowSurface;

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class DLLEXPORT PhysicalDevice : public ResourceBase{
public:
	PhysicalDevice(const std::string& name, Instance& instance, WindowSurface& windowSurface);

	bool checkThisDeviceExtensionSupport() const;

	QueueFamilyIndices findQueueFamiliesThisDevice();

	SwapChain::SwapChainSupportDetails querySwapChainSupportThisDevice() const;

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkPhysicalDevice& getRaw();
	
	bool isThisDeviceSuitable();

	VkSampleCountFlagBits getMsaaSamples();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	GENERATETYPE(PhysicalDevice)
private:
	VkSampleCountFlagBits getMaxUsableSampleCount();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
	bool isDeviceSuitable(VkPhysicalDevice device);
	SwapChain::SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkPhysicalDevice device;
	Instance& instance;
	WindowSurface& windowSurface;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};