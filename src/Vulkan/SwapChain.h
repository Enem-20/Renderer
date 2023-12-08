#pragma once

#include "Resources/ResourceBase.h"
#include "ImageView.h"

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


enum VkPresentModeKHR;
struct VkSurfaceFormatKHR;
struct VkExtent2D;
struct VkSurfaceCapabilitiesKHR;
enum VkFormat;
struct VkSemaphore_T;
typedef VkSemaphore_T* VkSemaphore;
struct VkImage_T;
struct VkImageView_T;
typedef VkImage_T* VkImage;
typedef VkImageView_T* VkImageView;
struct VkFramebuffer_T;
typedef VkFramebuffer_T* VkFramebuffer;
struct VkSwapchainKHR_T;
typedef VkSwapchainKHR_T* VkSwapchainKHR;

class SwapChain : public ResourceBase, public ImageView {
public:
	SwapChain() = delete;

	SwapChain(const std::string& name, WindowSurface& windowSurface, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
	SwapChain(const SwapChain& swapchain);
	//SwapChain(SwapChain&& swapchain);

	struct SwapChainSupportDetails {
		std::shared_ptr<VkSurfaceCapabilitiesKHR> capabilities = std::make_shared<VkSurfaceCapabilitiesKHR>();
		std::vector<VkSurfaceFormatKHR> formats;

		std::vector<VkPresentModeKHR> presentModes;
	};

	~SwapChain();
	
	VkFormat& getSwapChainImageFormat();
	VkExtent2D& getSwapchainExtent();
	std::vector<VkImageView>& getSwapChainImageViews();
	std::vector<VkFramebuffer>& getSwapChainFramebuffers();
	std::vector<VkImage>& getSwapChainImages();
	VkSwapchainKHR& getRaw();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats);

	VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);


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
	VkExtent2D& chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities);

	WindowSurface& currentWindowSurface;
	PhysicalDevice& currentPhysicalDevice;
	LogicalDevice& currentLogicalDevice;
	std::unique_ptr<DepthResources> depthResources;
	std::unique_ptr<ColorResources> colorResources;
	
	VkSwapchainKHR swapchain;

	
	std::shared_ptr<VkFormat> swapChainImageFormat;
	std::shared_ptr<VkExtent2D> swapChainExtent;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	std::vector<VkFramebuffer> swapChainFramebuffers;
};