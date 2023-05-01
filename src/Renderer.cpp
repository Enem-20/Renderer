#include "Renderer.h"

#include "Vulkan/Instance.h"
#include "Vulkan/DebugMessenger.h"
#include "Vulkan/WindowSurface.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/LogicalDevice.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/ImageView.h"
#include "Vulkan/RenderPipeline.h"
#include "Vulkan/CommandPool.h"
#include "Texture2D.h"
#include "Vulkan/VertexBuffer.h"
#include "Vulkan/IndexBuffer.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptionSets.h"
#include "Vulkan/DescriptorSetLayout.h"
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/SyncObjects.h"
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/GeneralVulkanStorage.h"


#include "WindowManager.h"
#include "Window.h"

void Renderer::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	WindowManager::init("Vulkan");

	glfwSetWindowUserPointer(WindowManager::GetCurrentWindow().GetRaw(), this);
	glfwSetFramebufferSizeCallback(WindowManager::GetCurrentWindow().GetRaw(), framebufferResizeCallback);
}

Renderer::Renderer(){
	initWindow();
	instance = std::make_unique<Instance>();
#ifdef GLFW_INCLUDE_VULKAN
	debugMessanger = std::make_unique<DebugMessanger>(*instance);
#endif // GLFW_INCLUDE_VULKAN
	windowSurface = std::make_unique<WindowSurface>(*instance);
	physicalDevice = std::make_unique<PhysicalDevice>(*instance, *windowSurface);
	logicalDevice = std::make_unique<LogicalDevice>(*windowSurface, *physicalDevice);
	swapchain = std::make_unique<SwapChain>(*windowSurface, *physicalDevice, *logicalDevice);
	swapchain->createImageViews();
	descriptorSetLayout = std::make_unique<DescriptorSetLayout>(*logicalDevice);
	renderPipeline = std::make_unique<RenderPipeline>(*logicalDevice, *swapchain, *descriptorSetLayout);
	swapchain->createFramebuffers(*renderPipeline);
	commandPool = std::make_unique<CommandPool>(*physicalDevice, *logicalDevice);
	texture = std::make_unique<Texture2D>(*physicalDevice, *logicalDevice, *commandPool);
	vertexBuffer = std::make_unique<VertexBuffer>(*logicalDevice, *commandPool);
	indexBuffer = std::make_unique<IndexBuffer>(*logicalDevice, *commandPool);
	uniformBuffers = std::make_unique<UniformBuffers>(*logicalDevice, *swapchain);
	descriptorPool = std::make_unique<DescriptorPool>(*logicalDevice, *commandPool);
	descriptionSets = std::make_unique<DescriptionSets>(*logicalDevice, *descriptorSetLayout, *descriptorPool, *uniformBuffers, *texture);
	commandBuffers = std::make_unique<CommandBuffers>(*logicalDevice, *commandPool, *renderPipeline, *swapchain, *vertexBuffer, *indexBuffer, *descriptionSets);
	syncObjects = std::make_unique<SyncObjects>(*logicalDevice);
}

//Renderer::~Renderer() {
//	swapchain.reset();
//	texture.reset();
//	uniformBuffers.reset();
//	descriptorPool.reset();
//	descriptorSetLayout.reset();
//	indexBuffer.reset();
//	vertexBuffer.reset();
//	renderPipeline.reset();
//	syncObjects.reset();
//	commandPool.reset();
//	logicalDevice.reset();
//	debugMessanger.reset();
//	windowSurface.reset();
//	instance.reset();
//}

void Renderer::render() {
#ifdef GLFW_INCLUDE_VULKAN
	while (!glfwWindowShouldClose(WindowManager::CurrentWindow->window)) {
		glfwPollEvents();
		drawFrame();
	}
	logicalDevice->wait();
#endif // GLFW_INCLUDE_VULKAN
}

void Renderer::drawFrame() {
	syncObjects->waitForFences(currentFrame);

	const auto& imageIndex = swapchain->acquireNextImage(*renderPipeline, syncObjects->getImageAvailableSemaphores(), currentFrame);
	if (imageIndex != -1) {
		syncObjects->resetFences(currentFrame);
		commandBuffers->resetCommandBuffer(currentFrame);
		uniformBuffers->updateUniformBuffer(currentFrame);
		commandBuffers->recordCommandBuffer(currentFrame, imageIndex);
		logicalDevice->queuePresent(*swapchain, *renderPipeline, *commandBuffers, *syncObjects, currentFrame, imageIndex, framebufferResized);

#ifdef GLFW_INCLUDE_VULKAN
		currentFrame = (currentFrame + 1) % GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT;
#endif
	}
}

//int main()
//{
//	Renderer renderer;
//	renderer.render();
//
//	return 0;
//}