#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

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