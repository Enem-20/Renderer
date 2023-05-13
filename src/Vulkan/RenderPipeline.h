#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include <vector>
#include <fstream>
#include <memory>

#include <GLFW\glfw3.h>

class LogicalDevice;
class SwapChain;
class DescriptorSetLayout;
class ShaderProgram;

class DLLEXPORT RenderPipeline : public ResourceBase{
public:
	RenderPipeline(const std::string& name, LogicalDevice& currentLogicalDevice, SwapChain& swapchain, DescriptorSetLayout& descriptorSetLayout);
	~RenderPipeline();

	VkRenderPass& getRenderPass();
	VkDescriptorSetLayout& getDescriptorSetLayout();
	VkPipeline& getGraphicsPipeline();
	VkPipelineLayout& getPipelineLayout();

	inline static const std::string type = GETTYPE(RenderPipeline);
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