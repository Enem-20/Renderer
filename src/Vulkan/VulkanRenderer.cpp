#include "VulkanRenderer.h"

#include "../../src/Resources/ResourceManager.h"
#include "../../src/GameTypes/GameObject.h"
#include "../Sprite.h"

#include "ImGui/ImGui.h"

#include "Instance.h"
#include "DebugMessenger.h"
#include "WindowSurface.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SwapChain.h"
#include "ImageView.h"
#include "RenderPass.h"
#include "RenderPipeline.h"
#include "CommandPool.h"
#include "../Texture2D.h"
#include "Resources/Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DescriptorPool.h"
#include "DescriptionSets.h"
#include "DescriptorSetLayout.h"
#include "CommandBuffer.h"
#include "SyncObjects.h"
#include "UniformBuffer.h"
#include "GeneralVulkanStorage.h"

#include "Vertex.h"

#include "../WindowManager.h"
#include "../Window.h"

#include <GLFW/glfw3.h>

void VulkanRenderer::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	WindowManager::init("Vulkan", 600, 600);

	glfwSetWindowUserPointer(WindowManager::GetCurrentWindow()->GetRaw(), this);
	std::function<void(GLFWwindow* window, int width, int height)> framebufferResizeCallback =
		[this](GLFWwindow* window, int width, int height) -> void {
		auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
		framebufferResized = true;
		};
	glfwSetFramebufferSizeCallback(WindowManager::GetCurrentWindow()->GetRaw(), framebufferResizeCallback.target<void(GLFWwindow * window, int width, int height)>());
}

VulkanRenderer::VulkanRenderer(const std::string& name)
	: BaseRenderer(name)
{
	initWindow();
	instance = ResourceManager::makeResource<Instance>(std::string("TestInstance"));
#ifdef GLFW_INCLUDE_VULKAN
	debugMessanger = ResourceManager::makeResource<DebugMessenger>("TestDebugMessenger", *instance);
#endif // GLFW_INCLUDE_VULKAN
	windowSurface = ResourceManager::makeResource<WindowSurface>("TestWindowSurface", *instance);
	physicalDevice = ResourceManager::makeResource<PhysicalDevice>("TestPhysicalDevice", *instance, *windowSurface);
	logicalDevice = ResourceManager::makeResource<LogicalDevice>("TestLogicalDevice", *windowSurface, *physicalDevice);
	swapchain = ResourceManager::makeResource<SwapChain>("TestSwapChain", *windowSurface, *physicalDevice, *logicalDevice);
	swapchain->createImageViews();

	UniformBuffers::createDescriptorSetLayout(*logicalDevice);
	Texture2D::createDescriptorSetLayout(*logicalDevice);

	ResourceManager::loadShadersReal();
	renderPass = ResourceManager::makeResource<RenderPass>("TestRenderPass", physicalDevice, logicalDevice, swapchain);
	renderPipeline = ResourceManager::makeResource<RenderPipeline, const std::string&, PhysicalDevice&, LogicalDevice&, SwapChain&, RenderPass&, const std::string&>("TestRenderPipeline", *physicalDevice, *logicalDevice, *swapchain, *renderPass, "TestShaderProgram");
	commandPool = ResourceManager::makeResource<CommandPool>("TestCommandPool", *physicalDevice, *logicalDevice);
	swapchain->createColorResources(*commandPool);
	swapchain->createDepthResources(*commandPool);
	swapchain->createFramebuffers(*(renderPass));
	descriptorPool = ResourceManager::makeResource<DescriptorPool>("TestDescriptorPool", *logicalDevice, *commandPool);
	commandBuffers = ResourceManager::makeResource<CommandBuffers>("TestCommandBuffers", *logicalDevice, *commandPool, renderPass, *swapchain);
	syncObjects = ResourceManager::makeResource<SyncObjects>("TestSyncObjects", *logicalDevice);

	VulkanRenderer::ViewportSize.x = swapchain->getSwapchainExtent().width;
	VulkanRenderer::ViewportSize.y = swapchain->getSwapchainExtent().height;

	ResourceManager::addResource<VulkanRenderer>(this);

	ResourceManager::onAfterRenderInitialization();
}

VulkanRenderer::~VulkanRenderer() {
	logicalDevice->wait();
	glfwTerminate();
}

void VulkanRenderer::render() {
	glfwPollEvents();
	drawFrame();
}

void VulkanRenderer::drawFrame() {
	ResourceManager::onBeforeRenderFrame();

	syncObjects->waitForFences(currentFrame);

	const auto& imageIndex = swapchain->acquireNextImage(*(renderPass), *commandPool, syncObjects->getImageAvailableSemaphores(), currentFrame);
	if (imageIndex != -1) {
		auto GameObjects = ResourceManager::getResourcesWithType<GameObject>();

		syncObjects->resetFences(currentFrame);
		commandBuffers->resetCommandBuffer(currentFrame);

		for (auto itGameObject = GameObjects->begin(); itGameObject != GameObjects->end(); ++itGameObject) {
			itGameObject->second.getResource<GameObject>()->Update(currentFrame);
		}

		commandBuffers->recordCommandBuffer(currentFrame, imageIndex, renderPipeline);
		logicalDevice->queuePresent(*swapchain, *(renderPass), *commandBuffers, *commandPool, *syncObjects, currentFrame, imageIndex, framebufferResized);

#ifdef GLFW_INCLUDE_VULKAN
		currentFrame = (currentFrame + 1) % GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT;
#endif
	}
}

void VulkanRenderer::addTexture(std::shared_ptr<Texture2D> texture) {
	textures.push_back(texture);
}
void VulkanRenderer::removeTexture(const std::string& name) {
	std::vector<std::shared_ptr<Texture2D>>::iterator whatRemove = textures.end();
	for (std::vector<std::shared_ptr<Texture2D>>::iterator it = textures.begin(); it != textures.end(); ++it) {
		if ((*it)->name == name)
			whatRemove = it;
	}

	if (whatRemove != textures.end())
		textures.erase(whatRemove);
}

void VulkanRenderer::recreatePipeline(const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners, std::vector<std::function<void()>> onAfterListeners) {
	std::function<void()> onBefore = [this, shaderName, onBeforeListeners, onAfterListeners]() {recreatePipelineReal(shaderName, onBeforeListeners, onAfterListeners); };
	beforeFrameEventListeners.push(onBefore);
}

void VulkanRenderer::recreatePipelineReal(const std::string& shaderName, std::vector<std::function<void()>> onBeforeListeners, std::vector<std::function<void()>> onAfterListeners) {
	std::string name = "TestRenderPipeline";
	auto pipeline = renderPipeline;
	if (pipeline)
		std::string name = pipeline->name;
	pipeline.reset();
	renderPipeline.reset();
	ResourceManager::removeResource<RenderPipeline>(name);
	renderPipeline = ResourceManager::makeResource<RenderPipeline>(name, *physicalDevice, *logicalDevice, *swapchain, *renderPass, shaderName, onBeforeListeners, onAfterListeners);
}

void VulkanRenderer::OnBeforeFrame() {
	while (!beforeFrameEventListeners.empty()) {
		auto currentOnBeforeListener = beforeFrameEventListeners.front();
		currentOnBeforeListener();

		beforeFrameEventListeners.pop();
	}
}

//int main()
//{
//	VulkanRenderer VulkanRenderer;
//	VulkanRenderer.render();
//
//	return 0;
//}