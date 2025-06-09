#ifndef C_RENDERPASS_H
#define C_RENDERPASS_H

#include <cstddef>
#include <string_view>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "Resources/ResourceBase.h"


class Device;
class SwapChain;

class Attachments;

class RenderPass : public ResourceBase{
public:
	RenderPass(std::string_view name, Device* device);
	~RenderPass();

	VkRenderPass getRenderPass();

	GENERATETYPE(RenderPass)
private:
	Attachments* attachements;
	
	Device* device;

	VkRenderPass renderPass;
};

#endif