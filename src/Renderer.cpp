#include "Renderer.h"

#include "../../src/Resources/ResourceManager.h"
#include "../../src/GameTypes/GameObject.h"
#include "Sprite.h"

#include "ImGui/ImGui.h"

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
#include "../../../src/Resources/Mesh.h"
#include "Vulkan/VertexBuffer.h"
#include "Vulkan/IndexBuffer.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptionSets.h"
#include "Vulkan/DescriptorSetLayout.h"
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/SyncObjects.h"
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/GeneralVulkanStorage.h"

#include "Vulkan/Vertex.h"

#include "WindowManager.h"
#include "Window.h"

glm::vec2 Renderer::ViewportSize;

void Renderer::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	WindowManager::init("Vulkan");

	glfwSetWindowUserPointer(WindowManager::GetCurrentWindow()->GetRaw(), this);
	glfwSetFramebufferSizeCallback(WindowManager::GetCurrentWindow()->GetRaw(), framebufferResizeCallback);
}

Renderer::Renderer(const std::string& name)
	: ResourceBase(name)
{
	initWindow(); 
	//auto t = std::make_shared<Instance>("");
	instance = ResourceManager::makeResource<Instance>(std::string("TestInstance"));
#ifdef GLFW_INCLUDE_VULKAN
	debugMessanger = ResourceManager::makeResource<DebugMessenger>("TestDebugMessenger", * instance);
#endif // GLFW_INCLUDE_VULKAN
	windowSurface = ResourceManager::makeResource<WindowSurface>("TestWindowSurface", * instance);
	physicalDevice = ResourceManager::makeResource<PhysicalDevice>("TestPhysicalDevice", * instance, *windowSurface);
	logicalDevice = ResourceManager::makeResource<LogicalDevice>("TestLogicalDevice", * windowSurface, *physicalDevice);
	swapchain = ResourceManager::makeResource<SwapChain>("TestSwapChain", * windowSurface, *physicalDevice, *logicalDevice);
	swapchain->createImageViews();
	
	UniformBuffers::createDescriptorSetLayout(*logicalDevice);
	Texture2D::createDescriptorSetLayout(*logicalDevice);
	ResourceManager::loadShadersReal();
	renderPipeline = ResourceManager::makeResource<RenderPipeline>("TestRenderPipeline", *physicalDevice, * logicalDevice, *swapchain);
	commandPool = ResourceManager::makeResource<CommandPool>("TestCommandPool", *physicalDevice, *logicalDevice);
	swapchain->createColorResources(*commandPool);
	swapchain->createDepthResources(*commandPool);
	swapchain->createFramebuffers(* renderPipeline);
	descriptorPool = ResourceManager::makeResource<DescriptorPool>("TestDescriptorPool", * logicalDevice, *commandPool);
	commandBuffers = ResourceManager::makeResource<CommandBuffers>("TestCommandBuffers", * logicalDevice, *commandPool, *renderPipeline, *swapchain);
	syncObjects = ResourceManager::makeResource<SyncObjects>("TestSyncObjects", * logicalDevice);

	Renderer::ViewportSize.x = swapchain->getSwapchainExtent().width;
	Renderer::ViewportSize.y = swapchain->getSwapchainExtent().height;

	ResourceManager::addResource<Renderer>(this);
}

Renderer::~Renderer() {
	//ResourceManager::removeResource<Renderer>(name);
}

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

	const auto& imageIndex = swapchain->acquireNextImage(*renderPipeline, *commandPool, syncObjects->getImageAvailableSemaphores(), currentFrame);
	if (imageIndex != -1) {
		auto GameObjects = ResourceManager::getResourcesWithType<GameObject>();

		syncObjects->resetFences(currentFrame);
		commandBuffers->resetCommandBuffer(currentFrame);

		for (auto& itGameObject : *GameObjects) {
			itGameObject.second.getResource<GameObject>()->Update(currentFrame);
		}
		
		commandBuffers->recordCommandBuffer(currentFrame, imageIndex);
		logicalDevice->queuePresent(*swapchain, *renderPipeline, *commandBuffers,*commandPool, *syncObjects, currentFrame, imageIndex, framebufferResized);

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