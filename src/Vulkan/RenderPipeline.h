#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include "../../../EventSystem/src/InitializeEventsInterface.h"

#include <vector>
#include <fstream>
#include <memory>
#include <functional>
#include <optional>

#include <GLFW\glfw3.h>

class PhysicalDevice;
class LogicalDevice;
class SwapChain;
class DescriptorSetLayout;
class ShaderProgram;
class RenderPass;

class DLLEXPORT RenderPipeline : public ResourceBase, public InitializeEventsInterface{
public:
	RenderPipeline(const std::string& name, PhysicalDevice& physicalDevice, LogicalDevice& currentLogicalDevice, SwapChain& swapchain, RenderPass& renderPass, const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners = {}, std::vector<std::function<void()>> onAfterListeners = {});
	~RenderPipeline();

	//VkRenderPass& getRenderPass();
	VkPipeline& getGraphicsPipeline();
	VkPipelineLayout& getPipelineLayout();

	std::string shaderName;

	GENERATETYPE(RenderPipeline)
private:
	//void createRenderPass();
	static std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	VkPipeline graphicsPipeline;
	std::optional<VkPipelineLayout> pipelineLayout;
	//VkRenderPass renderPass;

	//PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;
	//SwapChain& swapchain;
};