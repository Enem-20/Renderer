#include "ColorResources.h"

#include "CommandPool.h"
#include "SwapChain.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

ColorResources::ColorResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, SwapChain& swapchain)
	: ImageProcessing(physicalDevice, logicalDevice, commandPool)
{
	VkFormat colorFormat = swapchain.getSwapChainImageFormat();

	createImage(swapchain.getSwapchainExtent().width, swapchain.getSwapchainExtent().height, 1, physicalDevice.getMsaaSamples(), colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory);
	imageView = createImageView(image, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

ColorResources::~ColorResources() {

}