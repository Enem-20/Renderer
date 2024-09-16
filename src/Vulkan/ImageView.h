#pragma once

#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"

#include <memory>
#include <vector>

typedef uint32_t VkFlags;
typedef VkFlags VkImageAspectFlags;
enum VkFormat;
struct VkImage_T;
struct VkImageView_T;
typedef VkImage_T* VkImage;
typedef VkImageView_T* VkImageView;

class LogicalDevice;

class DLLEXPORT ImageView {
public:
	ImageView(LogicalDevice& logicalDevice);

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	LogicalDevice& logicalDevice;
};

#endif