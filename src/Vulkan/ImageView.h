#pragma once

#include "GLFW\glfw3.h"

#include <memory>
#include <vector>

class LogicalDevice;

class ImageView {
public:
	ImageView(std::shared_ptr<LogicalDevice> logicalDevice, VkImage image, VkFormat format);
	VkImageView getRaw();
private:
	std::shared_ptr<LogicalDevice> logicalDevice;
	VkImageView imageView;
};