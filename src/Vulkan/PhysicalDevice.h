#pragma once

#include "Capturer.h"
#include "Instance.h"
#include "SwapChain.h"

#include "API/ExportPropety.h"

#include <optional>
#include <memory>

class WindowSurface;

#define VK_KHR_SWAPCHAIN_EXTENSION_NAME   "VK_KHR_swapchain"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class SwapChain;
struct SwapChain::SwapChainSupportDetails;

typedef uint32_t VkFlags;
typedef VkFlags VkMemoryPropertyFlags;
typedef VkFlags VkFormatFeatureFlags;


struct VkDevice_T;
typedef VkDevice_T* VkDevice;
struct VkPhysicalDevice_T;
typedef VkPhysicalDevice_T* VkPhysicalDevice;

enum VkSampleCountFlagBits;
enum VkFormat;
enum VkImageTiling;

class DLLEXPORT PhysicalDevice : public ResourceBase{
public:
	PhysicalDevice(const std::string& name, Instance& instance, WindowSurface& windowSurface);

	bool checkThisDeviceExtensionSupport() const;

	QueueFamilyIndices findQueueFamiliesThisDevice();

	SwapChain::SwapChainSupportDetails querySwapChainSupportThisDevice() const;

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags& properties);

	VkPhysicalDevice& getRaw();
	
	bool isThisDeviceSuitable();

	std::shared_ptr<VkSampleCountFlagBits> getMsaaSamples();
	std::shared_ptr<VkFormat> findSupportedFormat(const std::vector < std::shared_ptr<VkFormat>>& candidates, std::shared_ptr<VkImageTiling> tiling, VkFormatFeatureFlags features);
	std::shared_ptr<VkFormat> findDepthFormat();

	static inline const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	GENERATETYPE(PhysicalDevice)
private:
	std::shared_ptr<VkSampleCountFlagBits> getMaxUsableSampleCount();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
	bool isDeviceSuitable(VkPhysicalDevice device);
	std::shared_ptr<SwapChain::SwapChainSupportDetails> querySwapChainSupport(VkPhysicalDevice device) const;
	std::shared_ptr<QueueFamilyIndices> findQueueFamilies(VkPhysicalDevice device);

	VkPhysicalDevice device;
	Instance& instance;
	WindowSurface& windowSurface;
};