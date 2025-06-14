#include "Renderer.h"

#include "Resources/ResourceManager.h"
#include "GameTypes/GameObject.h"
#include "Sprite.h"

#include "ImGui/ImGui.h"

#include "Vulkan/Instance.h"
#include "Vulkan/DebugMessenger.h"
#include "Vulkan/WindowSurface.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/LogicalDevice.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/ImageView.h"
#include "Vulkan/RenderPass.h"
#include "Vulkan/RenderPipeline.h"
#include "Vulkan/CommandPool.h"
#include "Texture2D.h"
#include "Resources/Mesh.h"
#include "Vulkan/VertexBuffer.h"
#include "Vulkan/IndexBuffer.h"
#include "Vulkan/DescriptorPool.h"
#include "Vulkan/DescriptionSets.h"
#include "Vulkan/DescriptorSetLayout.h"
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/SyncObjects.h"
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/GeneralVulkanStorage.h"
#include "Vulkan/Device.h"

#include "Vulkan/Vertex.h"

#include "WindowManager.h"
#include "Window.h"

#include <GLFW/glfw3.h>
#include <memory>

glm::vec2 Renderer::ViewportSize;

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	Renderer::framebufferResized = true;
}

void Renderer::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	WindowManager::init("Vulkan", 600, 600);

	glfwSetWindowUserPointer(WindowManager::GetCurrentWindow()->GetRaw(), this);
	glfwSetFramebufferSizeCallback(WindowManager::GetCurrentWindow()->GetRaw(), framebufferResizeCallback);
}

Renderer::Renderer(const std::string& name)
	: ResourceBase(name)
{
	initWindow();
	instance = ResourceManager::makeResource<Instance>(std::string("TestInstance"));
#ifdef GLFW_INCLUDE_VULKAN
	debugMessanger = ResourceManager::makeResource<DebugMessenger>("TestDebugMessenger", *instance);
#endif // GLFW_INCLUDE_VULKAN
	windowSurface = ResourceManager::makeResource<WindowSurface>("TestWindowSurface", *instance);
	physicalDevice.reset(&windowSurface->getSupportedDevice("d0")->device);
	logicalDevice.reset(&windowSurface->getSupportedDevice("d0")->logicalDevice);
	swapchain = ResourceManager::makeResource<SwapChain>("TestSwapChain", *windowSurface, *physicalDevice, *logicalDevice);
	swapchain->createImageViews();

	UniformBuffers::createDescriptorSetLayout(*logicalDevice);
	Texture2D::createDescriptorSetLayout(*logicalDevice);

	ResourceManager::loadJSONShaders("res/shaders/");
	renderPass = ResourceManager::makeResource<RenderPass>("TestRenderPass", physicalDevice, logicalDevice, swapchain);
	renderPipeline = ResourceManager::makeResource<RenderPipeline, const std::string&, PhysicalDevice&, LogicalDevice&, SwapChain&, RenderPass&, const std::string&>("TestRenderPipeline", *physicalDevice, *logicalDevice, *swapchain, *renderPass, "TestShaderProgram");
	commandPool = ResourceManager::makeResource<CommandPool>("TestCommandPool", windowSurface->getSupportedDevice("d0"));
	swapchain->createColorResources(*commandPool);
	swapchain->createDepthResources(*commandPool);
	swapchain->createFramebuffers(* (renderPass));
	descriptorPool = ResourceManager::makeResource<DescriptorPool>("TestDescriptorPool", * logicalDevice, *commandPool);
	commandBuffers = ResourceManager::makeResource<CommandBuffers>("TestCommandBuffers", * logicalDevice, *commandPool, renderPass, *swapchain);
	syncObjects = ResourceManager::makeResource<SyncObjects>("TestSyncObjects", * logicalDevice);

	Renderer::ViewportSize.x = swapchain->getSwapchainExtent().width;
	Renderer::ViewportSize.y = swapchain->getSwapchainExtent().height;

	ResourceManager::addResource<Renderer>(this);

	ResourceManager::onAfterRenderInitialization();
}

Renderer::~Renderer() {
	glfwTerminate();
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
		logicalDevice->queuePresent(*swapchain, *(renderPass), *commandBuffers,*commandPool, *syncObjects, currentFrame, imageIndex, framebufferResized);

#ifdef GLFW_INCLUDE_VULKAN
		currentFrame = (currentFrame + 1) % GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT;
#endif
	}
}

void Renderer::addTexture(std::shared_ptr<Texture2D> texture) {
	textures.push_back(texture);
}
void Renderer::removeTexture(std::string_view name) {
	std::vector<std::shared_ptr<Texture2D>>::iterator whatRemove = textures.end();
	for (std::vector<std::shared_ptr<Texture2D>>::iterator it = textures.begin(); it != textures.end(); ++it) {
		if ((*it)->name == name)
			whatRemove = it;	
	}

	if (whatRemove != textures.end())
		textures.erase(whatRemove);
}

void Renderer::recreatePipeline(std::string_view shaderName, std::vector<std::function<void()>> onBeforeListeners, std::vector<std::function<void()>> onAfterListeners) {
	std::function<void()> onBefore = [this, shaderName, onBeforeListeners, onAfterListeners]() {recreatePipelineReal(shaderName, onBeforeListeners, onAfterListeners); };
	beforeFrameEventListeners.push(onBefore);
}

void Renderer::recreatePipelineReal(std::string_view shaderName, std::vector<std::function<void()>> onBeforeListeners, std::vector<std::function<void()>> onAfterListeners) {
	std::string name = "TestRenderPipeline";
	auto pipeline = renderPipeline;
	if(pipeline)
		std::string name = pipeline->name;
	pipeline.reset();
	renderPipeline.reset();
	ResourceManager::removeResource<RenderPipeline>(name);
	renderPipeline = ResourceManager::makeResource<RenderPipeline>(name, *physicalDevice, *logicalDevice, *swapchain, *renderPass, shaderName, onBeforeListeners, onAfterListeners);
}

void Renderer::OnBeforeFrame() {
	while (!beforeFrameEventListeners.empty()) {
		auto currentOnBeforeListener = beforeFrameEventListeners.front();
		currentOnBeforeListener();

		beforeFrameEventListeners.pop();
	}
}

//int main()
//{
//	Renderer renderer;
//	renderer.render();
//
//	return 0;
//}