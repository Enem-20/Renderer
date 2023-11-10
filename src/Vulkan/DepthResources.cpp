#include "DepthResources.h"

#include "CommandPool.h"
#include "SwapChain.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

#include <GLFW/glfw3.h>

DepthResources::DepthResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, SwapChain& swapchain)
	: ImageProcessing(physicalDevice, logicalDevice, commandPool)
	, swapchain(swapchain)
{
	VkFormat depthFormat = *(physicalDevice.findDepthFormat());
	createImage(swapchain.getSwapchainExtent().width, swapchain.getSwapchainExtent().height, 1, physicalDevice.getMsaaSamples(), depthFormat,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		image, imageMemory);
	imageView = createImageView(image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}

DepthResources::~DepthResources() {

}