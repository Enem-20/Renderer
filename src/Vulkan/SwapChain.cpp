#include "SwapChain.h"

#include "../ImGui/ImGui.h"

#include "ColorResources.h"
#include "DepthResources.h"
#include "RenderPass.h"
#include "ImageView.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "WindowSurface.h"

#include "../WindowManager.h"
#include "../Window.h"

#include "Resources/ResourceManager.h"

#include <GLFW/glfw3.h>

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
, ResourceBase(swapchain.name)
, ImageView(swapchain.currentLogicalDevice)
{

}

SwapChain::SwapChain(const std::string& name, WindowSurface& windowSurface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice)
: currentPhysicalDevice(physicalDevice)
, currentWindowSurface(windowSurface)
, currentLogicalDevice(logicalDevice)
, ResourceBase(name)
, ImageView(logicalDevice)
{
	create();

	ResourceManager::addResource<SwapChain>(this);
}

SwapChain::~SwapChain() {
	cleanupSwapChain();
}

void SwapChain::create() {
	SwapChainSupportDetails swapChainSupport = currentPhysicalDevice.querySwapChainSupportThisDevice();

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = chooseSwapExtent(*swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities->minImageCount + 1;

	if (swapChainSupport.capabilities->maxImageCount > 0 && imageCount > swapChainSupport.capabilities->maxImageCount) {
		imageCount = swapChainSupport.capabilities->maxImageCount;
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

	createInfo.preTransform = swapChainSupport.capabilities->currentTransform;
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

	swapChainImageFormat = std::make_shared<VkFormat>(surfaceFormat.format);
	swapChainExtent = std::make_shared<VkExtent2D>(extent);
}

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats) {
	for (auto& availableFormat : availableFormats) {

		if ((availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB) && (availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (auto availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D& SwapChain::chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(WindowManager::GetCurrentWindow()->GetRaw(), &width, &height);
		VkExtent2D* ext = new VkExtent2D{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};
		lastExtent = std::shared_ptr<VkExtent2D>(ext);

		lastExtent->width = std::clamp(lastExtent->width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		lastExtent->height = std::clamp(lastExtent->height, capabilities.maxImageExtent.height, capabilities.maxImageExtent.height);
		return *lastExtent;
	}
}

void SwapChain::recreateSwapChain(RenderPass& renderPass, CommandPool& commandPool) {
	int width = 0, height = 0;
	glfwGetFramebufferSize(WindowManager::GetCurrentWindow()->GetRaw(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetWindowSize(WindowManager::GetCurrentWindow()->GetRaw(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(currentLogicalDevice.getRaw());

	cleanupSwapChain();

	create();
	createImageViews();
	createColorResources(commandPool);
	createDepthResources(commandPool);
	createFramebuffers(renderPass);
}

void SwapChain::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); ++i) {
		swapChainImageViews[i] = createImageView(swapChainImages[i], *swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}

void SwapChain::createFramebuffers(RenderPass& renderPass) {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
		std::array<VkImageView, 3> attachments = {
			colorResources->getImageView(),
			depthResources->getImageView(),
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass.getRenderPass();
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent->width;
		framebufferInfo.height = swapChainExtent->height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(currentLogicalDevice.getRaw(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw("failed to create framebuffer!");
		}
	}
}

void SwapChain::createDepthResources(CommandPool& commandPool) {
	depthResources = std::make_unique<DepthResources>(currentPhysicalDevice, currentLogicalDevice, commandPool, *this);
}

void SwapChain::createColorResources(CommandPool& commandPool) {
	colorResources = std::make_unique<ColorResources>(currentPhysicalDevice, currentLogicalDevice, commandPool, *this);
}

VkFormat& SwapChain::getSwapChainImageFormat() {
	return *swapChainImageFormat;
}

VkExtent2D& SwapChain::getSwapchainExtent() {
	return *swapChainExtent;
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

uint32_t SwapChain::acquireNextImage(RenderPass& renderPass, CommandPool& commandPool, std::vector<VkSemaphore> imageAvailableSemaphores, uint32_t currentFrame) {
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(currentLogicalDevice.getRaw(), swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain(renderPass, commandPool);
		return -1;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	return imageIndex;
}

void SwapChain::cleanupSwapChain() {
	colorResources.reset();
	depthResources.reset();

	for (size_t i = 0; i < swapChainFramebuffers.size(); ++i) {
		vkDestroyFramebuffer(currentLogicalDevice.getRaw(), swapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
		vkDestroyImageView(currentLogicalDevice.getRaw(), swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(currentLogicalDevice.getRaw(), swapchain, nullptr);
}