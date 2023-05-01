#pragma once

#include <GLFW\glfw3.h>

#include <memory>
#include <vector>

class CommandPool;
class LogicalDevice;
class RenderPipeline;
class VertexBuffer;
class IndexBuffer;
class SwapChain;
class DescriptionSets;

struct CommandBuffers {
	CommandBuffers(LogicalDevice& logicalDevice, CommandPool& commandPool,
		RenderPipeline& renderPipeline, SwapChain& swapchain,
		VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer,
		DescriptionSets& descriptorSets);

	void resetCommandBuffer(uint32_t currentFrame);
	void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex);

	std::vector<VkCommandBuffer>& getRaw();
private:
	VertexBuffer& vertexBuffer;
	IndexBuffer& indexBuffer;
	SwapChain& swapchain;
	RenderPipeline& renderPipeline;
	DescriptionSets& descriptorSets;
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