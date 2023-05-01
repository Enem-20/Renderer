#include "CommandBuffer.h"

#include "DescriptionSets.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "RenderPipeline.h"
#include "SwapChain.h"
#include "CommandPool.h"
#include "LogicalDevice.h"
#include "GeneralVulkanStorage.h"


#include <iostream>

CommandBuffers::CommandBuffers(LogicalDevice& logicalDevice, CommandPool& commandPool,
	RenderPipeline& renderPipeline, SwapChain& swapchain,
	VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer,
	DescriptionSets& descriptorSets)
	: renderPipeline(renderPipeline)
	, swapchain(swapchain)
	, vertexBuffer(vertexBuffer)
	, indexBuffer(indexBuffer)
	, descriptorSets(descriptorSets)
{
	raw.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool.getRaw();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)raw.size();

	if (vkAllocateCommandBuffers(logicalDevice.getRaw(), &allocInfo, raw.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void CommandBuffers::resetCommandBuffer(uint32_t currentFrame) {
	vkResetCommandBuffer(raw[currentFrame], 0);
}

void CommandBuffers::recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex) {
	VkCommandBuffer commandBuffer = raw[currentFrame];

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPipeline.getRenderPass();
	renderPassInfo.framebuffer = swapchain.getSwapChainFramebuffers()[imageIndex];

	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapchain.getSwapchainExtent();

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderPipeline.getGraphicsPipeline());

	VkBuffer vertexBuffers[] = { vertexBuffer.getRaw()};
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer.getRaw(), 0, VK_INDEX_TYPE_UINT16);


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapchain.getSwapchainExtent().width);
	viewport.height = static_cast<float>(swapchain.getSwapchainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = swapchain.getSwapchainExtent();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderPipeline.getPipelineLayout(), 0, 1, &(descriptorSets.getDescriptorSets())[currentFrame], 0, nullptr);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indexBuffer.getIndices().size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

std::vector<VkCommandBuffer>& CommandBuffers::getRaw() {
	return raw;
}

CommandBuffer::CommandBuffer()
{
	//VkCommandBufferAllocateInfo allocInfo{};
	//allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	//allocInfo.commandPool = commandPool->getRaw();
	//allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	//allocInfo.commandBufferCount = 1;

	//if (vkAllocateCommandBuffers(logicalDevice->getRaw(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
	//	throw std::runtime_error("failed to allocate command buffers!");
	//}
}

CommandBuffer::CommandBuffer(const CommandBuffer& commandBuffer)
: commandBuffer(commandBuffer.commandBuffer)
{}

VkCommandBuffer& CommandBuffer::getRaw() {
	return commandBuffer;
}