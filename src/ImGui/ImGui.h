#pragma once

#ifndef IMGUI
#define IMGUI

#include "../../src/ExportPropety.h"


#ifdef SHOWONBUILD

#include <glfw/glfw3.h>

#else // SHOWONBUILD
struct VkRenderPass;
struct VkDescriptorPool;
struct VkCommandPool;
struct VkCommandBuffer;
struct VkFramebuffer;
#endif

#include <memory>
#include <vector>

class LogicalDevice;
class CommandBuffer;
class RenderPipeline;


class DLLEXPORT ImGuiManager {
public:
	static void init();
	static void destroy();
	ImGuiManager() = delete;
	~ImGuiManager() = delete;

	//void Start(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline);
	static void Update(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline);

	static void createRenderPass();
	static void createCommandPool();
	static void createCommandBuffers();
	static void createFrameBuffers();
private:
	
#if defined(GLFW_INCLUDE_VULKAN) && defined(SHOWONBUILD)
	static std::shared_ptr<LogicalDevice> logicalDevice;
	static VkRenderPass renderPass;
	static VkDescriptorPool imguiDescriptorPool;
	static VkCommandPool imguiCommandPool;
	static std::vector<VkCommandBuffer> commandBuffers;
	static std::vector<VkFramebuffer> frameBuffers;
#endif // GLFW_INCLUDE_VULKAN
};

#endif