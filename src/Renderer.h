#pragma once

#include "GLFW/glfw3.h"

#include <memory>

class Instance;
class DebugMessanger;
class WindowSurface;
class PhysicalDevice;
class LogicalDevice;
class SwapChain;
class RenderPipeline;
class CommandPool;
class Texture2D;
class VertexBuffer;
class IndexBuffer;
class UniformBuffers;
class DescriptorPool;
class DescriptionSets;
class CommandBuffers;
class SyncObjects;
class DescriptorSetLayout;

class Renderer {
public:
	Renderer();
	//~Renderer();
	void render();
	void drawFrame();

	bool framebufferResized = false;
private:
	void initWindow();	
	
	uint32_t currentFrame = 0;
	
	/////////////////////////Placed in deleting order/////////////////////////
	std::unique_ptr<Instance> instance;
	std::unique_ptr<WindowSurface> windowSurface;
	std::unique_ptr<DebugMessanger> debugMessanger;
	std::unique_ptr<LogicalDevice> logicalDevice;
	std::unique_ptr<CommandPool> commandPool;
	std::unique_ptr<SyncObjects> syncObjects;
	std::unique_ptr<RenderPipeline> renderPipeline;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
	std::unique_ptr<DescriptorPool> descriptorPool;
	std::unique_ptr<UniformBuffers> uniformBuffers;
	std::unique_ptr<Texture2D> texture;
	std::unique_ptr<SwapChain> swapchain;
	/////////////////////////////////////////////////////////////////////////

	///////////////////////////Without deleting order////////////////////////
	std::unique_ptr<DescriptionSets> descriptionSets;
	std::unique_ptr<CommandBuffers> commandBuffers;
	std::unique_ptr<PhysicalDevice> physicalDevice;
	////////////////////////////////////////////////////////////////////////
};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}