#pragma once



#ifndef IMGUI
#define IMGUI

#include "glfw/glfw3.h"

#include <memory>
#include <vector>

class LogicalDevice;
class CommandBuffer;
class RenderPipeline;


class ImGuiManager {
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
	
#ifdef GLFW_INCLUDE_VULKAN
	static std::shared_ptr<LogicalDevice> logicalDevice;
	static VkRenderPass renderPass;
	static VkDescriptorPool imguiDescriptorPool;
	static VkCommandPool imguiCommandPool;
	static std::vector<VkCommandBuffer> commandBuffers;
	static std::vector<VkFramebuffer> frameBuffers;
#endif // GLFW_INCLUDE_VULKAN
};

#endif