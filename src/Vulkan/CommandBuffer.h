#pragma once

#include "../../src/ExportPropety.h"

#include "../ImGui/ImGui.h"
#include "../../src/Resources/ResourceBase.h"


#include <GLFW\glfw3.h>

#include <memory>
#include <vector>
#include <functional>

class CommandPool;
class LogicalDevice;
class RenderPipeline;
class VertexBuffer;
class IndexBuffer;
class SwapChain;
class DescriptionSets;
//class ImGuiManager;

struct DLLEXPORT CommandBuffers : public ResourceBase {
	CommandBuffers(const std::string& name, LogicalDevice& logicalDevice, CommandPool& commandPool,
		RenderPipeline& renderPipeline, SwapChain& swapchain);

	void resetCommandBuffer(uint32_t currentFrame);
	void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex);

	std::vector<VkCommandBuffer>& getRaw();

	GENERATETYPE(CommandBuffers)
private:
	void drawIndexed(uint32_t currentFrame, VkCommandBuffer commandBuffer);

	//VertexBuffer& vertexBuffer;
	//IndexBuffer& indexBuffer;
	SwapChain& swapchain;
	RenderPipeline& renderPipeline;
	//DescriptionSets& descriptorSets;
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