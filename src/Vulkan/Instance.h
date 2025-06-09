#pragma once

#include "Renderer/src/Vulkan/SwapChain.h"
#ifndef C_INSTANCE_H
#define C_INSTANCE_H

//#include "API/ExportPropety.h"
#include <string_view>
#include <vector>
#include <cstdint>

#include "tsl/hopscotch_map.h"

#include "Resources/ResourceBase.h"
#include "VulkanObject.h"

#include "Renderer/src/Vulkan/PhysicalDevice.h"

struct VkInstance_T;
typedef VkInstance_T* VkInstance;

struct VkDebugUtilsMessengerCreateInfoEXT;

class DebugMessenger;
class WindowSurface;
class Device;
class Frame;
class RenderPass;

class Instance : public ResourceBase, public VulkanObject{
public:
	//enum class BufferizationMode : uint32_t {
	//	None = 1,
	//	Double = 2,
	//	Triple = 3
	//};
	Instance(const std::string& name, std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"}, VulkanObject* parent = nullptr);
	~Instance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	std::vector<const char*> getRequiredExtensions() const;
	bool checkValidationLayerSupport() const;
	VkInstance& getRaw();

	void addDevice(std::string_view name, Device* device);
	void removeDevice(std::string_view name);
	void addRenderPass(std::string_view name, RenderPass* renderPass);

	WindowSurface* getWindowSurface(std::string_view name);

	GENERATETYPE(Instance)

	uint32_t getMaxFramesInFlight() const;
	void setMaxFramesInFlight(uint32_t maxFramesInFlight);
	std::vector<Frame*>& getFramesInFlight();
	uint32_t getCurrentFrame() const;
	const std::vector<const char*>& getValidationLayers() const;
private:
	uint32_t maxFramesInFlight = 2;
	uint32_t currentFrame = 0;
	VkInstance instance;
	WindowSurface* mainWindowSurface = nullptr;

	std::vector<const char*> validationLayers;
	std::vector<Frame*> framesInFlight;
	tsl::hopscotch_map<std::string, Device*> devices;
	tsl::hopscotch_map<std::string, RenderPass*> renderPasses;
	tsl::hopscotch_map<std::string, WindowSurface*> windowSurfaces;
	tsl::hopscotch_map<std::string, DebugMessenger*> debugMessengers;
};

#endif