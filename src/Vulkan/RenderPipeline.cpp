#include "RenderPipeline.h"

#include "../../src/Resources/ResourceManager.h"

#include "../ShaderProgram.h"
#include "Vertex.h"
#include "RenderPass.h"
#include "DescriptorSetLayout.h"
#include "SwapChain.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"


#include <iostream>
#include <string>

RenderPipeline::RenderPipeline(const std::string& name, PhysicalDevice& physicalDevice, LogicalDevice& currentLogicalDevice, SwapChain& swapchain, RenderPass& renderPass, const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners, std::vector<std::function<void()>> onAfterListeners)
	: logicalDevice(currentLogicalDevice)
	, shaderName(shaderName)
	, ResourceBase(name)
	, InitializeEventsInterface(onBeforeListeners, onAfterListeners)
{
	OnBeforeInitialize();
	//auto vertShaderCode = readFile("C:/Projects/VulkanTest/build/Debug/res/defaultShaders/vert.spv");
	//auto fragShaderCode = readFile("C:/Projects/VulkanTest/build/Debug/res/defaultShaders/frag.spv");

	//VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	//VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	auto shaderProgram = ResourceManager::getResource<ShaderProgram>(shaderName);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = shaderProgram->getVertexShaderModule();
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = shaderProgram->getFragmentShaderModule();
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)(swapchain.getSwapchainExtent().width);
	viewport.height = (float)(swapchain.getSwapchainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = swapchain.getSwapchainExtent();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = /*VK_FRONT_FACE_CLOCKWISE;*/VK_FRONT_FACE_COUNTER_CLOCKWISE;

	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE;
	multisampling.rasterizationSamples = physicalDevice.getMsaaSamples();
	multisampling.minSampleShading = .2f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT
		| VK_COLOR_COMPONENT_G_BIT
		| VK_COLOR_COMPONENT_B_BIT
		| VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts = { UniformBuffers::getDescriptorSetLayout(), Texture2D::getDescriptorSetLayout() };

	if (!pipelineLayout.has_value()) {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		
		VkPipelineLayout layout;
		VkResult pipelineLayoutCreatingResult = vkCreatePipelineLayout(logicalDevice.getRaw(), &pipelineLayoutInfo, nullptr, &layout);

		pipelineLayout = layout;

		if (pipelineLayoutCreatingResult != VK_SUCCESS) {
			std::cout << "Result: " << std::to_string(pipelineLayoutCreatingResult);
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}


	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {};
	depthStencil.back = {};

	//createRenderPass();

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.pDepthStencilState = &depthStencil;

	pipelineInfo.layout = pipelineLayout.value();
	pipelineInfo.renderPass = renderPass.getRenderPass();
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	VkResult pipelineCreatingResult = vkCreateGraphicsPipelines(logicalDevice.getRaw(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);

	if (pipelineCreatingResult != VK_SUCCESS) {
		std::cout << "Result: " + std::to_string(pipelineCreatingResult);
		throw std::runtime_error("failed to create graphics pipline! Result:" + std::to_string(pipelineCreatingResult));
	}

	vkDestroyShaderModule(logicalDevice.getRaw(), shaderProgram->getVertexShaderModule(), nullptr);
	vkDestroyShaderModule(logicalDevice.getRaw(), shaderProgram->getFragmentShaderModule(), nullptr);

	OnAfterInitialize();

	ResourceManager::addResource<RenderPipeline>(this);
}

RenderPipeline::~RenderPipeline() {
	//ResourceManager::removeResource<RenderPipeline>(name);
	vkDestroyPipeline(logicalDevice.getRaw(), graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice.getRaw(), pipelineLayout.value(), nullptr);

	//vkDestroyRenderPass(logicalDevice.getRaw(), renderPass, nullptr);
}

//VkRenderPass& RenderPipeline::getRenderPass() {
//	return renderPass;
//}

VkPipeline& RenderPipeline::getGraphicsPipeline() {
	return graphicsPipeline;
}

//void RenderPipeline::createRenderPass() {
//	VkAttachmentDescription colorAttachment{};
//	colorAttachment.format = swapchain.getSwapChainImageFormat();
//	colorAttachment.samples = physicalDevice.getMsaaSamples();
//	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//
//	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//
//	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentDescription depthAttachment{};
//	depthAttachment.format = physicalDevice.findDepthFormat();
//	depthAttachment.samples = physicalDevice.getMsaaSamples();
//	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentDescription colorAttachmentResolve{};
//	colorAttachmentResolve.format = swapchain.getSwapChainImageFormat();
//	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
//	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//	VkAttachmentReference colorAttachmentRef{};
//	colorAttachmentRef.attachment = 0;
//	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentReference depthAttachmentRef{};
//	depthAttachmentRef.attachment = 1;
//	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//
//	VkAttachmentReference colorAttachmentResolveRef{};
//	colorAttachmentResolveRef.attachment = 2;
//	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	VkSubpassDescription subpass{};
//	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//	subpass.colorAttachmentCount = 1;
//	subpass.pColorAttachments = &colorAttachmentRef;
//	subpass.pDepthStencilAttachment = &depthAttachmentRef;
//	subpass.pResolveAttachments = &colorAttachmentResolveRef;
//
//	VkSubpassDependency dependency{};
//	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//	dependency.dstSubpass = 0;
//
//	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//	dependency.srcAccessMask = 0;
//
//	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//
//	std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
//	VkRenderPassCreateInfo renderPassInfo{};
//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
//	renderPassInfo.pAttachments = attachments.data();
//	renderPassInfo.subpassCount = 1;
//	renderPassInfo.pSubpasses = &subpass;
//	renderPassInfo.dependencyCount = 1;
//	renderPassInfo.pDependencies = &dependency;
//
//	if (vkCreateRenderPass(logicalDevice.getRaw(), &renderPassInfo, nullptr, &renderPass)) {
//		throw std::runtime_error("failed to create render pass!");
//	}
//}

VkPipelineLayout& RenderPipeline::getPipelineLayout() {
	return pipelineLayout.value();
}

std::vector<char> RenderPipeline::readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

VkShaderModule RenderPipeline::createShaderModule(const std::vector<char>& code) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(logicalDevice.getRaw(), &createInfo, nullptr, &shaderModule)) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}