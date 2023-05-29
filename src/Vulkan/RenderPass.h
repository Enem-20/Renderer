#pragma once

#ifndef RENDERPASS
#define RENDERPASS

#include "../../src/Resources/ResourceBase.h"

#include <glfw/glfw3.h>

#include <memory>
#include <string>

class PhysicalDevice;
class LogicalDevice;
class SwapChain;

class RenderPass : public ResourceBase{
public:
	RenderPass(const std::string& name, std::shared_ptr<PhysicalDevice> physicalDevice, std::shared_ptr<LogicalDevice> logicalDevice, std::shared_ptr<SwapChain> swapchain);
	~RenderPass();

	VkRenderPass getRenderPass();

	GENERATETYPE(RenderPass)
private:
	std::weak_ptr<LogicalDevice> logicalDevice;

	VkRenderPass renderPass;
};

#endif