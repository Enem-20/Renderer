#include "RenderPass.h"

#include <array>
#include <cstring>
#include <string>


#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "ImageView.h"
#include "Resources/ResourceManager.h"

#include "SwapChain.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Device.h"

RenderPass::RenderPass(std::string_view name, Device* device)
	: ResourceBase(name)
	, device(device)
{
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = attachmentCount;
	subpass.pColorAttachments = attachements.colorAttachmentRefs.data();
	subpass.pDepthStencilAttachment = attachements.depthAttachmentRefs.data();
	subpass.pResolveAttachments = attachements.colorAttachmentResolveRefs.data();

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, attachements.colorAttachments.size() + attachements.depthAttachments.size() + attachements.colorAttachmentResolves.size()> _attachments;
	std::memcpy(_attachments.data(), attachements.colorAttachments.data(), attachements.colorAttachments.size() * sizeof(VkAttachmentDescription));
	std::memcpy(_attachments.data() + attachements.colorAttachments.size(), attachements.depthAttachments.data(), attachements.depthAttachments.size() * sizeof(VkAttachmentDescription));
	std::memcpy(_attachments.data() + attachements.colorAttachments.size() + attachements.depthAttachments.size(), attachements.colorAttachmentResolves.data(), attachements.colorAttachmentResolves.size() * sizeof(VkAttachmentDescription));

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(_attachments.size());
	renderPassInfo.pAttachments = _attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device->logicalDevice.getRaw(), &renderPassInfo, nullptr, &renderPass)) {
		throw std::runtime_error("failed to create render pass!");
	}

	ResourceManager::addResource(this);
}

template<size_t attachmentCount>
RenderPass<attachmentCount>::~RenderPass() {
	vkDestroyRenderPass(device->logicalDevice.getRaw(), renderPass, nullptr);
}

template<size_t attachmentCount>
VkRenderPass RenderPass<attachmentCount>::getRenderPass() {
	return renderPass;
}