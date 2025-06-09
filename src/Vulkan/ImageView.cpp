#include "ImageView.h"

#include "PhysicalDevice.h"
#include "Device.h"
#include "Image.h"
#include <cstdint>

Attachment::Attachment(VkFormat format, size_t samplesCount, VkImageLayout layout, size_t layoutIndex) {
	attachment.format = format;
	if(layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	else {
		if(samplesCount == 0)
			
		switch (samplesCount) {
		case 0:
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			break;
		case 1:
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			break;
		default:
			if((samplesCount < 65) && (samplesCount % 2 == 0)) {
				attachment.samples = static_cast<VkSampleCountFlagBits>(samplesCount);
			} else {
				throw std::runtime_error("Invalid sample count for attachment: " + std::to_string(samplesCount));
			}
		}
	}
		

	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout = layout;

	attachmentRef.attachment = static_cast<uint32_t>(layoutIndex);
	if(layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		attachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	else
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

ImageView::ImageView(Device* logicalDevice, Image* image, uint32_t mipLevels, VkImageAspectFlags aspectFlags) 
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image->getRaw();
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = image->getRawFormat();

	viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(logicalDevice->logicalDevice.getRaw(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}	
}

VkImageView ImageView::getRaw()  {
	return imageView;
}