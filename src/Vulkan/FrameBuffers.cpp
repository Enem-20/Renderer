#include "FrameBuffers.h"
#include <cstddef>

#include "RenderPass.h"
#include "ImageView.h"
#include "ImageViews.h"

FrameBuffers::FrameBuffers(size_t count, RenderPass* renderPass, ImageViews* attachments) 
    : framebuffers(count)
{
    for(size_t i = 0; i < count; ++i) {
        VkFramebufferCreateInfo framebufferInfo;
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->getRenderPass();
		auto _attachments = attachments->getRaw();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(_attachments.size());
		framebufferInfo.pAttachments = _attachments.data();
		framebufferInfo.width = swapChainExtent->width;
		framebufferInfo.height = swapChainExtent->height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(currentLogicalDevice->getRaw(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw("failed to create framebuffer!");
		}
    }
}

FrameBuffers::~FrameBuffers() {

}

std::vector<VkFramebuffer>& FrameBuffers::getFramebuffers() {
    return framebuffers;
}