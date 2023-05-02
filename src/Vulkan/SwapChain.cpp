#include "SwapChain.h"

#include "RenderPipeline.h"
#include "ImageView.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "WindowSurface.h"

#include "../WindowManager.h"
#include "../Window.h"

#include <algorithm>

SwapChain::SwapChain(const SwapChain& swapchain) 
: currentPhysicalDevice(swapchain.currentPhysicalDevice)
, currentWindowSurface(swapchain.currentWindowSurface)
, currentLogicalDevice(swapchain.currentLogicalDevice)
, swapchain(swapchain.swapchain)
, swapChainImages(swapchain.swapChainImages)
, swapChainImageFormat(swapchain.swapChainImageFormat)
, swapChainExtent(swapchain.swapChainExtent)
, swapChainImageViews(swapchain.swapChainImageViews) 
{

}

//SwapChain::SwapChain(SwapChain&& swapchain) 
//{
//
//}

SwapChain::SwapChain(WindowSurface& windowSurface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice)
: currentPhysicalDevice(physicalDevice)
, currentWindowSurface(windowSurface)
, currentLogicalDevice(logicalDevice)
{
	create();
}

SwapChain::~SwapChain() {
	cleanupSwapChain();
}

void SwapChain::create() {
	SwapChainSupportDetails swapChainSupport = currentPhysicalDevice.querySwapChainSupportThisDevice();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = currentWindowSurface.getRaw();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = currentPhysicalDevice.findQueueFamiliesThisDevice();
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(currentLogicalDevice.getRaw(), &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(currentLogicalDevice.getRaw(), swapchain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(currentLogicalDevice.getRaw(), swapchain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	for (const auto& availableFormat : availableFormats) {

		if ((availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB) && (availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(WindowManager::GetCurrentWindow().GetRaw(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.maxImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
}

void SwapChain::recreateSwapChain(RenderPipeline& renderPipeline) {
	int width = 0, height = 0;
	glfwGetFramebufferSize(WindowManager::GetCurrentWindow().GetRaw(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetWindowSize(WindowManager::GetCurrentWindow().GetRaw(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(currentLogicalDevice.getRaw());

	cleanupSwapChain();

	create();
	createImageViews();
	createFramebuffers(renderPipeline);
}

void SwapChain::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); ++i) {
		swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat);
	}
}

void SwapChain::createFramebuffers(RenderPipeline& renderPipeline) {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
		VkImageView attachments[] = {
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPipeline.getRenderPass();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(currentLogicalDevice.getRaw(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw("failed to create framebuffer!");
		}
	}
}

VkImageView SwapChain::createImageView(VkImage image, VkFormat format) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;

	viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(currentLogicalDevice.getRaw(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

VkFormat SwapChain::getSwapChainImageFormat() const {
	return swapChainImageFormat;
}

VkExtent2D SwapChain::getSwapchainExtent() {
	return swapChainExtent;
}

std::vector<VkImageView>& SwapChain::getSwapChainImageViews() {
	return swapChainImageViews;
}

std::vector<VkFramebuffer>& SwapChain::getSwapChainFramebuffers() {
	return swapChainFramebuffers;
}

std::vector<VkImage>& SwapChain::getSwapChainImages() {
	return swapChainImages;
}

VkSwapchainKHR& SwapChain::getRaw() {
	return swapchain;
}

uint32_t SwapChain::acquireNextImage(RenderPipeline& renderPipeline, std::vector<VkSemaphore> imageAvailableSemaphores, uint32_t currentFrame) {
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(currentLogicalDevice.getRaw(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain(renderPipeline);
		return -1;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	return imageIndex;
}

void SwapChain::cleanupSwapChain() {
	for (size_t i = 0; i < swapChainFramebuffers.size(); ++i) {
		vkDestroyFramebuffer(currentLogicalDevice.getRaw(), swapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
		vkDestroyImageView(currentLogicalDevice.getRaw(), swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(currentLogicalDevice.getRaw(), swapchain, nullptr);
}

void SwapChain::resetDependencies() {
}