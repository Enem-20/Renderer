#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include <vector>
#include <fstream>
#include <memory>

#include <GLFW\glfw3.h>

class PhysicalDevice;
class LogicalDevice;
class SwapChain;
class DescriptorSetLayout;
class ShaderProgram;

class DLLEXPORT RenderPipeline : public ResourceBase{
public:
	RenderPipeline(const std::string& name, PhysicalDevice& physicalDevice, LogicalDevice& currentLogicalDevice, SwapChain& swapchain);
	~RenderPipeline();

	VkRenderPass& getRenderPass();
	VkPipeline& getGraphicsPipeline();
	VkPipelineLayout& getPipelineLayout();

	GENERATETYPE(RenderPipeline)
private:
	void createRenderPass();
	static std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;
	SwapChain& swapchain;
};