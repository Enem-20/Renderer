#pragma once

#include "../../src/Resources/ResourceBase.h"
#include "ImageView.h"

#include <GLFW/glfw3.h>

#include <optional>
#include <memory>
#include <vector>

class ColorResources;
class DepthResources;
class CommandPool;
class RenderPass;
class LogicalDevice;
class PhysicalDevice;
class WindowSurface;


class SwapChain : public ResourceBase, public ImageView {
public:
	SwapChain() = delete;

	SwapChain(const std::string& name, WindowSurface& windowSurface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	SwapChain(const SwapChain& swapchain);
	//SwapChain(SwapChain&& swapchain);

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;

		std::vector<VkPresentModeKHR> presentModes;
	};

	~SwapChain();
	
	VkFormat getSwapChainImageFormat() const;
	VkExtent2D getSwapchainExtent();
	std::vector<VkImageView>& getSwapChainImageViews();
	std::vector<VkFramebuffer>& getSwapChainFramebuffers();
	std::vector<VkImage>& getSwapChainImages();
	VkSwapchainKHR& getRaw();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);


	void createImageViews();
	void createFramebuffers(RenderPass& renderPass);
	void createDepthResources(CommandPool& commandPool);
	void createColorResources(CommandPool& commandPool);

	void recreateSwapChain(RenderPass& renderPass, CommandPool& commandPool);

	
	uint32_t acquireNextImage(RenderPass& renderPass,  CommandPool& commandPool, std::vector<VkSemaphore> imageAvailableSemaphores, uint32_t currentFrame);


	void cleanupSwapChain();

	GENERATETYPE(SwapChain)
private:
	void create();
	VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR capabilities);

	WindowSurface& currentWindowSurface;
	PhysicalDevice& currentPhysicalDevice;
	LogicalDevice& currentLogicalDevice;
	std::unique_ptr<DepthResources> depthResources;
	std::unique_ptr<ColorResources> colorResources;
	
	VkSwapchainKHR swapchain;

	
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	std::vector<VkFramebuffer> swapChainFramebuffers;
};