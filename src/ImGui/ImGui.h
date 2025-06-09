#pragma once

#include "Renderer/src/Vulkan/WindowSurface.h"
#ifndef IMGUI_H
#define IMGUI_H

#include "API/ExportPropety.h"


#include <memory>
#include <vector>

class LogicalDevice;
class CommandBuffer;
class RenderPipeline;

struct VkRenderPass_T;
typedef VkRenderPass_T* VkRenderPass;
struct VkDescriptorPool_T;
typedef VkDescriptorPool_T* VkDescriptorPool;
struct VkCommandPool_T;
typedef VkCommandPool_T* VkCommandPool;
struct VkCommandBuffer_T;
typedef VkCommandBuffer_T* VkCommandBuffer;
struct VkFramebuffer_T;
typedef VkFramebuffer_T* VkFramebuffer;

class DLLEXPORT ImGuiManager {
public:
	static void init(WindowSurface* windowSurface);
	static void destroy();
	ImGuiManager() = delete;
	~ImGuiManager() = delete;

	static void Update(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline);

	static void createRenderPass();
	static void createCommandPool();
	static void createCommandBuffers();
	static void createFrameBuffers();
private:
	
#if defined(GLFW_INCLUDE_VULKAN)
	static WindowSurface* windowSurface;
	static std::shared_ptr<LogicalDevice> logicalDevice;
	static VkRenderPass renderPass;
	static VkDescriptorPool imguiDescriptorPool;
	static VkCommandPool imguiCommandPool;
	static std::vector<VkCommandBuffer> commandBuffers;
	static std::vector<VkFramebuffer> frameBuffers;
#endif // GLFW_INCLUDE_VULKAN
};

#endif