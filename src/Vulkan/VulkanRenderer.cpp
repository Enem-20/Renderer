#include "VulkanRenderer.h"

#include "Resources/ResourceManager.h"
#include "GameTypes/GameObject.h"
#include "../RendererManager.h"
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
#include "VulkanTexture2D.h"
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

#include<unordered_map>

#include <GLFW/glfw3.h>

void VulkanRenderer::initWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	WindowManager::init("Vulkan", 600, 600);

	glfwSetWindowUserPointer(WindowManager::GetCurrentWindow()->GetRaw(), this);
	std::function<void(GLFWwindow* window, int width, int height)> framebufferResizeCallback =
		[this](GLFWwindow* window, int width, int height) -> void {
		auto app = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
		framebufferResized = true;
		};
	glfwSetFramebufferSizeCallback(WindowManager::GetCurrentWindow()->GetRaw(), framebufferResizeCallback.target<void(GLFWwindow * window, int width, int height)>());
}

VulkanRenderer::VulkanRenderer(const std::string& name)
	: BaseRenderer(name/*, this*/)
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
	VulkanTexture2D::createDescriptorSetLayout(*logicalDevice);

	//ResourceManager::loadShadersReal();
	RendererManager::applyShaderLoaders();
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

std::shared_ptr<BaseTexture2D> VulkanRenderer::createTexture(std::string_view name, const std::string& relativePath, const uint32_t& texWidth, const uint32_t& texHeight, unsigned char* pixels, const uint32_t& texChannels) {
	BaseTexture2D* texture = new VulkanTexture2D(name, relativePath, texWidth, texHeight, pixels, texChannels, std::unordered_map<size_t, std::string_view>
	{{SwapChain::type_hash, swapchain->name}, { PhysicalDevice::type_hash, physicalDevice->name }, { LogicalDevice::type_hash, logicalDevice->name }, { CommandPool::type_hash, commandPool->name }});
	ResourceManager::addResource<BaseTexture2D>(texture);
	return textures.emplace_back(ResourceManager::getResource<BaseTexture2D>(name));
}

void VulkanRenderer::addTexture(std::shared_ptr<BaseTexture2D> texture) {
	textures.push_back(texture);
}
void VulkanRenderer::removeTexture(std::string_view name) {
	std::remove_if(textures.begin(), textures.end(), [&name](std::shared_ptr<BaseTexture2D> tex) {return (tex->name == name); });
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

bool VulkanRenderer::windowShouldClose() const {
	bool result = glfwWindowShouldClose(WindowManager::GetCurrentWindow()->GetRaw());
	if (result)
		logicalDevice->wait();
	return result;
}

void VulkanRenderer::OnBeforeFrame() {
	while (!beforeFrameEventListeners.empty()) {
		auto currentOnBeforeListener = beforeFrameEventListeners.front();
		currentOnBeforeListener();

		beforeFrameEventListeners.pop();
	}
}

void VulkanRenderer::afterComplete() {
	logicalDevice->wait();
}




std::shared_ptr<Instance> VulkanRenderer::getInstance() const {
	return instance;
}

std::shared_ptr<WindowSurface> VulkanRenderer::getWindowSurface() const {
	return windowSurface;
}

std::shared_ptr<DebugMessenger> VulkanRenderer::getDebugMessanger() const {
	return debugMessanger;
}

std::shared_ptr<LogicalDevice> VulkanRenderer::getLogicalDevice() const {
	return logicalDevice;
}

std::shared_ptr<CommandPool> VulkanRenderer::getCommandPool() const {
	return commandPool;
}

std::shared_ptr<SyncObjects> VulkanRenderer::getSyncObjects() const {
	return syncObjects;
}

std::shared_ptr<RenderPass> VulkanRenderer::getRenderPass() const {
	return renderPass;
}

std::shared_ptr<RenderPipeline> VulkanRenderer::getRenderPipeline() const {
	return renderPipeline;
}

std::shared_ptr<DescriptorPool> VulkanRenderer::getDescriptorPool() const {
	return descriptorPool;
}

std::vector<std::shared_ptr<BaseTexture2D>> VulkanRenderer::getTextures() const {
	return textures;
}

std::shared_ptr<SwapChain> VulkanRenderer::getSwapchain() const {
	return swapchain;
}

std::shared_ptr<DescriptionSets> VulkanRenderer::getDescriptionSets() const {
	return descriptionSets;
}

std::shared_ptr<CommandBuffers> VulkanRenderer::getCommandBuffers() const {
	return commandBuffers;
}

std::shared_ptr<PhysicalDevice> VulkanRenderer::getPhysicalDevice() const {
	return physicalDevice;
}

//int main()
//{
//	VulkanRenderer VulkanRenderer;
//	VulkanRenderer.render();
//
//	return 0;
//}