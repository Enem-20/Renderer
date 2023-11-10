#include "CommandBuffer.h"

#include "Resources/ResourceManager.h"
#include "GameTypes/GameObject.h"
#include "../ImGui/ImGui.h"
#include "UI/UIelement.h"
#include "UI/Panel.h"
#include "../WindowManager.h"
#include "../Window.h"

#include "UniformBuffer.h"
#include "../Texture2D.h"
#include "DescriptionSets.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "RenderPipeline.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "CommandPool.h"
#include "LogicalDevice.h"
#include "GeneralVulkanStorage.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>

#include <GLFW/glfw3.h>

#include <unordered_map>
#include <iostream>

CommandBuffers::CommandBuffers(const std::string& name, LogicalDevice& logicalDevice, CommandPool& commandPool,
	std::shared_ptr<RenderPass> renderPass, SwapChain& swapchain)
	: renderPass(renderPass)
	, swapchain(swapchain)
	, ResourceBase(name)
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

	ResourceManager::addResource<CommandBuffers>(this);
}

CommandBuffers::~CommandBuffers() {

}

void CommandBuffers::resetCommandBuffer(uint32_t currentFrame) {
	vkResetCommandBuffer(raw[currentFrame], 0);
}

void CommandBuffers::recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, std::shared_ptr<RenderPipeline> renderPipeline) {
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
	renderPassInfo.renderPass = renderPass.lock()->getRenderPass();
	renderPassInfo.framebuffer = swapchain.getSwapChainFramebuffers()[imageIndex];

	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = swapchain.getSwapchainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	//VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderPipeline->getGraphicsPipeline());

	drawIndexed(currentFrame, commandBuffer, renderPipeline);

#ifdef GLFW_INCLUDE_VULKAN
	ImGui_ImplVulkan_NewFrame();
#elif OGL
	ImGui_ImplOpenGL3_NewFrame();
#endif
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	auto gameObjects = ResourceManager::getResourcesWithType<GameObject>();
	if(gameObjects)
		for (auto gameObject : *gameObjects) {
			auto panels = gameObject.second.getResource<GameObject>()->getComponentsWithType<Panel>();

			if(panels)
				for (auto panel : *panels) {
					panel.second.getComponentFromView<Panel>()->Update(currentFrame);
				}
		}

	ImGui::Render();
	CommandBuffer shellCommandBuffer;
	shellCommandBuffer.getRaw() = commandBuffer;
	ImGuiManager::Update(shellCommandBuffer, *(renderPipeline));

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void CommandBuffers::drawIndexed(uint32_t currentFrame, VkCommandBuffer commandBuffer, std::shared_ptr<RenderPipeline> renderPipeline) {
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

	auto gameObjects = ResourceManager::getResourcesWithType<GameObject>();

	CommandBuffer commandBufferWrapper;
	commandBufferWrapper.getRaw() = commandBuffer;
	for (auto& itGameObject : *gameObjects) {
		itGameObject.second.getResource<GameObject>()->render(commandBufferWrapper, *(renderPipeline), currentFrame);
	}
}

std::vector<VkCommandBuffer>& CommandBuffers::getRaw() {
	return raw;
}

CommandBuffer::CommandBuffer()
{
}

CommandBuffer::CommandBuffer(const CommandBuffer& commandBuffer)
	: commandBuffer(commandBuffer.commandBuffer)
{}

VkCommandBuffer& CommandBuffer::getRaw() {
	return commandBuffer;
}