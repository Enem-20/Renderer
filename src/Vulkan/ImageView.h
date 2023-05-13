#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include "GLFW\glfw3.h"

#include <memory>
#include <vector>

class LogicalDevice;

class DLLEXPORT ImageView : public ResourceBase {
public:
	ImageView(const std::string& name, std::shared_ptr<LogicalDevice> logicalDevice, VkImage image, VkFormat format);
	VkImageView getRaw();

	inline static const std::string type = GETTYPE(ImageView);
private:
	std::shared_ptr<LogicalDevice> logicalDevice;
	VkImageView imageView;
};