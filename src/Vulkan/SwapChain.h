#pragma once

#include "../DependencyOwner.h"

#include <GLFW/glfw3.h>

#include <optional>
#include <memory>
#include <vector>

class WindowSurface;
class PhysicalDevice;
class LogicalDevice;
class RenderPipeline;


class SwapChain : public DependencyOwner{
public:
	SwapChain() = delete;

	SwapChain(WindowSurface& windowSurface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
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
	void createFramebuffers(RenderPipeline& renderPipeline);

	void recreateSwapChain(RenderPipeline& renderPipeline);

	VkImageView createImageView(VkImage image, VkFormat format);

	
	uint32_t acquireNextImage(RenderPipeline& renderPipeline, std::vector<VkSemaphore> imageAvailableSemaphores, uint32_t currentFrame);


	void cleanupSwapChain();

	void resetDependencies() override;
private:
	void create();
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	WindowSurface& currentWindowSurface;
	PhysicalDevice& currentPhysicalDevice;
	LogicalDevice& currentLogicalDevice;
	VkSwapchainKHR swapchain;

	
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	std::vector<VkFramebuffer> swapChainFramebuffers;
};