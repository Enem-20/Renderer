#pragma once

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"

#include "EventSystem/InitializeEventsInterface.h"

#include <vector>
#include <fstream>
#include <memory>
#include <functional>
#include <optional>

class PhysicalDevice;
class LogicalDevice;
class SwapChain;
class DescriptorSetLayout;
class ShaderProgram;
class RenderPass;

struct VkShaderModule_T;
typedef VkShaderModule_T* VkShaderModule;
struct VkPipeline_T;
typedef VkPipeline_T* VkPipeline;
struct VkPipelineLayout_T;
typedef VkPipelineLayout_T* VkPipelineLayout;

class DLLEXPORT RenderPipeline : public ResourceBase, public InitializeEventsInterface{
public:
	RenderPipeline(const std::string& name, PhysicalDevice& physicalDevice, LogicalDevice& currentLogicalDevice, SwapChain& swapchain, RenderPass& renderPass, const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners = {}, std::vector<std::function<void()>> onAfterListeners = {});
	~RenderPipeline();

	VkPipeline& getGraphicsPipeline();
	VkPipelineLayout& getPipelineLayout();

	std::string shaderName;

	GENERATETYPE(RenderPipeline)
private:
	static std::vector<char> readFile(const std::string& filename);

	VkShaderModule createShaderModule(const std::vector<char>& code);

	VkPipeline graphicsPipeline;
	std::optional<VkPipelineLayout> pipelineLayout;
	LogicalDevice& logicalDevice;
};