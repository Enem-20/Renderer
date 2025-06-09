#include "PhysicalDevice.h"

#include "WindowSurface.h"
#include "Device.h"

#define VK_KHR_SWAPCHAIN_EXTENSION_NAME   "VK_KHR_swapchain"
PhysicalDevice::PhysicalDevice(std::string_view name, VkPhysicalDevice device, Device* owner, const std::vector<const char*> deviceExtensions)
	: ResourceBase(name)
	, queueFamilyCount(0)
	, queueFamilies(queueFamilyCount)
	, deviceExtensions(deviceExtensions)
	, device(device)
	, owner(owner)
{
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	queueFamilies.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
}

size_t PhysicalDevice::getMsaaSamples() const {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return  VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return  VK_SAMPLE_COUNT_2_BIT; }

	return  VK_SAMPLE_COUNT_1_BIT;
}

void PhysicalDevice::findQueueFamilies(WindowSurface* windowSurface) {
	if(windowSurface) {
    	for (uint32_t i = 0; i < queueFamilyCount; ++i) {
    	    RenderableQueueFamilyIndices queueFamilyIndices;
			if (presentSupport(windowSurface, i)) {
				queueFamilyIndices.presentFamily = i;
    	        presentIndices.push_back(i);
    	    }

			if(graphicsSupport(i)) {
				queueFamilyIndices.graphicsFamily = i;
				graphicsIndices.push_back(i);
			}

    	    if(queueFamilyIndices.isComplete()) {
		        renderableIndices.push_back(i);
		    }

			if(transferSupport(i)) {
				transferIndices.push_back(i);
			}

			if(computeSupport(i)) {
				computeIndices.push_back(i);
			}
    	}    
	}
}

bool PhysicalDevice::presentSupport(WindowSurface* windowSurface, uint32_t familyIndex) const {
	VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, familyIndex, windowSurface->getRaw(), &presentSupport);
	return presentSupport;
}

bool PhysicalDevice::graphicsSupport(uint32_t familyIndex) const {
	return queueFamilies[familyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

bool PhysicalDevice::transferSupport(uint32_t familyIndex) const {
	return queueFamilies[familyIndex].queueFlags & VK_QUEUE_TRANSFER_BIT;
}

bool PhysicalDevice::computeSupport(uint32_t familyIndex) const {
	return queueFamilies[familyIndex].queueFlags & VK_QUEUE_COMPUTE_BIT;
}

const std::vector<const char*>& PhysicalDevice::getDeviceExtensions() const {
	return deviceExtensions;
}

Device* PhysicalDevice::getOwner() const { return owner; }

const std::vector<uint32_t>& PhysicalDevice::getPresentIndices() const { return presentIndices; }
const std::vector<uint32_t>& PhysicalDevice::getGraphicsIndices() const { return graphicsIndices; }
const std::vector<uint32_t>& PhysicalDevice::getRenderableIndices() const { return renderableIndices; }
const std::vector<uint32_t>& PhysicalDevice::getTransferIndices() const { return transferIndices;}
const std::vector<uint32_t>& PhysicalDevice::getComputeIndices() const { return computeIndices; }

bool PhysicalDevice::isDiscrete() const {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

bool PhysicalDevice::isRenderSupport() const {
	return renderableIndices.size() > 0;
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

VkPhysicalDevice PhysicalDevice::getRaw() {
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