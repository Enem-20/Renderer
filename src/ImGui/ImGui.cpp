#include "ImGui.h"

#include "../../../src/Resources/ResourceManager.h"
#include "../../../UI/src/UIelement.h"
#include "../WindowManager.h"
#include "../Window.h"

#ifdef GLFW_INCLUDE_VULKAN
#include "../Vulkan/RenderPipeline.h" 
#include "../Vulkan/SingleTimeBuffer.h"
#include "../Vulkan/CommandBuffer.h"
#include "../Vulkan/CommandPool.h"
#include "../Vulkan/SwapChain.h"
#include "../Vulkan/DebugMessenger.h"
#include "../Vulkan/GeneralVulkanStorage.h"
#endif

#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN)
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#endif

#ifdef OGL
#include <imgui/backends/imgui_impl_opengl3.h>
#endif

#ifdef GLFW_INCLUDE_VULKAN
#include <imgui/backends/imgui_impl_vulkan.h>
#endif


std::shared_ptr<LogicalDevice> ImGuiManager::logicalDevice;
VkRenderPass ImGuiManager::renderPass;
VkDescriptorPool ImGuiManager::imguiDescriptorPool;
VkCommandPool ImGuiManager::imguiCommandPool;
std::vector<VkCommandBuffer> ImGuiManager::commandBuffers;
std::vector<VkFramebuffer> ImGuiManager::frameBuffers;

void ImGuiManager::init() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	auto currentWindow = WindowManager::GetCurrentWindow();
	logicalDevice = ResourceManager::getResource<LogicalDevice>("TestLogicalDevice");
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;

	if (vkCreateDescriptorPool(logicalDevice->getRaw(), &pool_info, nullptr, &imguiDescriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("Error during creating descriptor pool for imgui");
	}

	//createRenderPass();
	//createCommandPool();
	//createCommandBuffers();
	//createFrameBuffers();

	/*io.DisplaySize.x = static_cast<float>(currentWindow.size.x);
	io.DisplaySize.y = static_cast<float>(currentWindow.size.y);*/

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForVulkan(currentWindow->GetRaw(), true);

	auto physicalDevice = ResourceManager::getResource<PhysicalDevice>("TestPhysicalDevice");
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = ResourceManager::getResource<Instance>("TestInstance")->getRaw();
	init_info.PhysicalDevice = physicalDevice->getRaw();
	init_info.Device = logicalDevice->getRaw();
	init_info.QueueFamily = physicalDevice->findQueueFamiliesThisDevice().graphicsFamily.value();
	init_info.Queue = logicalDevice->getGraphicsQueue();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = imguiDescriptorPool;
	init_info.Subpass = 0;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

	renderPass = ResourceManager::getResource<RenderPipeline>("TestRenderPipeline")->getRenderPass();
	//createRenderPass();
	ImGui_ImplVulkan_Init(&init_info, renderPass);

	{
		auto commandPool = ResourceManager::getResource<CommandPool>("TestCommandPool");
		auto commandBuffer = SingleTimeBuffer(*logicalDevice, *commandPool);
		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer.getCommandBuffer().getRaw());
	}
	logicalDevice->queueWaitIdleGraphics();
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImGuiManager::destroy() {
	vkDestroyDescriptorPool(logicalDevice->getRaw(), imguiDescriptorPool, nullptr);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::createRenderPass() {
	auto swapchain = ResourceManager::getResource<SwapChain>("TestSwapChain");

	VkAttachmentDescription attachment = {};
	attachment.format = swapchain->getSwapChainImageFormat();
	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment = {};
	color_attachment.attachment = 0;
	color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	info.attachmentCount = 1;
	info.pAttachments = &attachment;
	info.subpassCount = 1;
	info.pSubpasses = &subpass;
	info.dependencyCount = 1;
	info.pDependencies = &dependency;

	if (vkCreateRenderPass(logicalDevice->getRaw(), &info, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("Could not create Dear ImGui's render pass");
	}
}

void ImGuiManager::createCommandPool() {
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = ResourceManager::getResource<PhysicalDevice>("TestPhysicalDevice")->findQueueFamiliesThisDevice().graphicsFamily.value();

	if (vkCreateCommandPool(logicalDevice->getRaw(), &poolInfo, nullptr, &imguiCommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void ImGuiManager::createCommandBuffers() {
	auto swapchain = ResourceManager::getResource<SwapChain>("TestSwapChain");
	commandBuffers.resize(swapchain->getSwapChainImageViews().size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = imguiCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(logicalDevice->getRaw(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void ImGuiManager::createFrameBuffers() {
	auto swapchain = ResourceManager::getResource<SwapChain>("TestSwapChain");
	frameBuffers.resize(swapchain->getSwapChainImageViews().size());
	VkImageView attachment[1];
	VkFramebufferCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	info.renderPass = renderPass;
	info.attachmentCount = 1;
	info.pAttachments = attachment;
	info.width = swapchain->getSwapchainExtent().width;
	info.height = swapchain->getSwapchainExtent().height;
	info.layers = 1;
	for (uint32_t i = 0; i < swapchain->getSwapChainImageViews().size(); i++)
	{
		attachment[0] = swapchain->getSwapChainImageViews()[i];
		if (vkCreateFramebuffer(logicalDevice->getRaw(), &info, nullptr, &frameBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

//void ImGuiManager::Start(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline) {
//	auto currentWindow = WindowManager::CurrentWindow;
//
//#ifdef GLFW_INCLUDE_VULKAN
//	ImGui_ImplVulkan_NewFrame();
//#elif OGL
//	ImGui_ImplOpenGL3_NewFrame();
//#endif
//
//	ImGui_ImplGlfw_NewFrame();
//	ImGui::NewFrame(); 
//
//	//ImGui::ShowDemoWindow();
//	for (auto& it : currentWindow->UIs)
//		it.second->Start();
//
//	ImGui::Render();
//
//	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.getRaw(), renderPipeline.getGraphicsPipeline());
//#ifdef GLFW_INCLUDE_VULKAN
//
//#elif OGL
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//	glfwSwapBuffers(window);
//#endif
//}


void ImGuiManager::Update(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline) {
	auto currentWindow = WindowManager::CurrentWindow;
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize.x = static_cast<float>(currentWindow->size.x);
	io.DisplaySize.y = static_cast<float>(currentWindow->size.y);

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.getRaw(), renderPipeline.getGraphicsPipeline());

#ifdef GLFW_INCLUDE_VULKAN
	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ResourceManager::getResource<CommandBuffers>("TestCommandBuffers")->getRaw()[currentFrame], ResourceManager::getResource<RenderPipeline>("TestRenderPipeline")->getGraphicsPipeline()); //Paste command buffer here
#elif OGL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
#endif
}