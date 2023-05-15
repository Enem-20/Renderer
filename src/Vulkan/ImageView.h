#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include "GLFW\glfw3.h"

#include <memory>
#include <vector>

class LogicalDevice;

class DLLEXPORT ImageView {
public:
	ImageView(LogicalDevice& logicalDevice);

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	LogicalDevice& logicalDevice;
};