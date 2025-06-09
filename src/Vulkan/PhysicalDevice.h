#pragma once

#ifndef C_PHYSICAL_DEVICE_H
#define C_PHYSICAL_DEVICE_H

#include <optional>
#include <memory>
#include <cstdint>
#include <vector>

#include "vulkan/vulkan.h"

#include "API/ExportPropety.h"

#include "Resources/ResourceBase.h"



struct RenderableQueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class SwapChain;
class Device;
class WindowSurface;

class DLLEXPORT PhysicalDevice : public ResourceBase{
public:
	PhysicalDevice(std::string_view name, VkPhysicalDevice device, Device* owner, const std::vector<const char*> deviceExtensions = {});

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags& properties);

	VkPhysicalDevice getRaw();

	size_t getMsaaSamples() const;
	
	std::shared_ptr<VkFormat> findSupportedFormat(const std::vector<std::shared_ptr<VkFormat>>& candidates, std::shared_ptr<VkImageTiling> tiling, VkFormatFeatureFlags features);
	std::shared_ptr<VkFormat> findDepthFormat();

	void findQueueFamilies(WindowSurface* windowSurface);
	bool presentSupport(WindowSurface* windowSurface, uint32_t familyIndex) const;
	bool graphicsSupport(uint32_t familyIndex) const;
	bool transferSupport(uint32_t familyIndex) const;
	bool computeSupport(uint32_t familyIndex) const;

	const std::vector<const char*>& getDeviceExtensions() const;
	Device* getOwner() const;
	const std::vector<uint32_t>& getPresentIndices() const;
	const std::vector<uint32_t>& getGraphicsIndices() const;
	const std::vector<uint32_t>& getRenderableIndices() const;
	const std::vector<uint32_t>& getTransferIndices() const;
	const std::vector<uint32_t>& getComputeIndices() const;

	bool isDiscrete() const;
	bool isRenderSupport() const;

	GENERATETYPE(PhysicalDevice)
private:
	uint32_t queueFamilyCount = 0;
	std::vector<VkQueueFamilyProperties> queueFamilies;
	std::vector<uint32_t> presentIndices;
	std::vector<uint32_t> graphicsIndices;
	std::vector<uint32_t> transferIndices;
	std::vector<uint32_t> computeIndices;
	std::vector<uint32_t> renderableIndices;
	//RenderableQueueFamilyIndices renderableQueueFamilyIndices;
	const std::vector<const char*> deviceExtensions;
	VkPhysicalDevice device;
	Device* owner;
};

#endif