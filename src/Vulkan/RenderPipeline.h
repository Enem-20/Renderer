#pragma once

#include <vector>
#include <fstream>
#include <memory>

#include <GLFW\glfw3.h>

class LogicalDevice;
class SwapChain;
class DescriptorSetLayout;

class RenderPipeline {
public:
	RenderPipeline(LogicalDevice& currentLogicalDevice, SwapChain& swapchain, DescriptorSetLayout& descriptorSetLayout);
	~RenderPipeline();

	VkRenderPass& getRenderPass();
	VkDescriptorSetLayout& getDescriptorSetLayout();
	VkPipeline& getGraphicsPipeline();
	VkPipelineLayout& getPipelineLayout();
private:
	void createRenderPass();
	static std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;

	LogicalDevice& logicalDevice;
	SwapChain& swapchain;
};