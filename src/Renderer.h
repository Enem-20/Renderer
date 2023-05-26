#pragma once

#include "../../src/Resources/ResourceBase.h"

#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include <vector>
#include <memory>
#include <string>

class Sprite;
class Instance;
class DebugMessenger;
class WindowSurface;
class PhysicalDevice;
class LogicalDevice;
class SwapChain;
class RenderPipeline;
class CommandPool;
class Texture2D;
class Mesh;
class VertexBuffer;
class IndexBuffer;
class UniformBuffers;
class DescriptorPool;
class DescriptionSets;
class CommandBuffers;
class SyncObjects;
class DescriptorSetLayout;

class Renderer : public ResourceBase{
public:
	Renderer(const std::string& name);
	~Renderer();

	void render();
	void awake();
	void start();
	void drawFrame();



	bool framebufferResized = false;

	void initWindow();	
	
	uint32_t currentFrame = 0;
	
	/////////////////////////Placed in deleting order/////////////////////////
	std::shared_ptr<Instance> instance;
	std::shared_ptr<WindowSurface> windowSurface;
	std::shared_ptr<DebugMessenger> debugMessanger;
	std::shared_ptr<LogicalDevice> logicalDevice;
	std::shared_ptr<CommandPool> commandPool;
	std::shared_ptr<SyncObjects> syncObjects;
	std::shared_ptr<RenderPipeline> renderPipeline;
	//std::shared_ptr<VertexBuffer> vertexBuffer;
	//std::shared_ptr<IndexBuffer> indexBuffer;
	//std::shared_ptr<Mesh> mesh;
	//std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
	std::shared_ptr<DescriptorPool> descriptorPool;
	std::vector<std::shared_ptr<Texture2D>> textures;
	std::shared_ptr<SwapChain> swapchain;
	/////////////////////////////////////////////////////////////////////////

	///////////////////////////Without deleting order////////////////////////
	std::shared_ptr<DescriptionSets> descriptionSets;
	std::shared_ptr<CommandBuffers> commandBuffers;
	std::shared_ptr<PhysicalDevice> physicalDevice;
	////////////////////////////////////////////////////////////////////////

	static glm::vec2 ViewportSize;

	GENERATETYPE(Renderer)
};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}