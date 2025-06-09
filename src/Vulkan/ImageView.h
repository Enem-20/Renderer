#pragma once


#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <cstddef>
#include <vulkan/vulkan.h>


class Device;
class Image;

struct Attachment {
	Attachment(VkFormat format, size_t samplesCount, VkImageLayout layout, size_t layoutIndex = 0);
	VkAttachmentDescription attachment{};
	VkAttachmentReference attachmentRef{};
	size_t index{0};
};

class ImageView {
public:
	ImageView(Device* logicalDevice, Image* image, uint32_t mipLevels, VkImageAspectFlags aspectFlags);
	VkImageView getRaw(); 
private:
	VkImageView imageView;
};

#endif