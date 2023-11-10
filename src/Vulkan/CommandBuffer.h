#pragma once

#ifndef COMMANDBUFFER
#define COMMANDBUFFER

#include "../../src/ExportPropety.h"

#include "../../src/Resources/ResourceBase.h"

#include <memory>
#include <vector>
#include <functional>

class CommandPool;
class LogicalDevice;
class RenderPipeline;
class RenderPass;
class VertexBuffer;
class IndexBuffer;
class SwapChain;
class DescriptionSets;

struct VkCommandBuffer_T;
typedef VkCommandBuffer_T* VkCommandBuffer;

//class ImGuiManager;

struct DLLEXPORT CommandBuffers : public ResourceBase {
	CommandBuffers(const std::string& name, LogicalDevice& logicalDevice, CommandPool& commandPool,
		std::shared_ptr<RenderPass> renderPass, SwapChain& swapchain);

	~CommandBuffers();

	void resetCommandBuffer(uint32_t currentFrame);
	void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, std::shared_ptr<RenderPipeline> renderPipeline);

	std::vector<VkCommandBuffer>& getRaw();

	GENERATETYPE(CommandBuffers)
private:
	void drawIndexed(uint32_t currentFrame, VkCommandBuffer commandBuffer, std::shared_ptr<RenderPipeline> renderPipeline);

	SwapChain& swapchain;
	std::weak_ptr<RenderPass> renderPass;
	std::vector<VkCommandBuffer> raw;
};

class CommandBuffer {
public:
	CommandBuffer();

	VkCommandBuffer& getRaw();
private:
	CommandBuffer(const CommandBuffer& commandBuffer);
	VkCommandBuffer commandBuffer;
};

#endif // !COMMANDBUFFER