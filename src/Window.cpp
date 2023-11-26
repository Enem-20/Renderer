#include "Window.h"

#include "UI/Panel.h"
#include "UI/UIelement.h"


#include "Resources/ResourceManager.h"
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/CommandPool.h"


#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN)
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#endif

#ifdef OGL
#include <imgui/backends/imgui_impl_opengl3.h>
#endif

#ifdef GLFW_INCLUDE_VULKAN
#include "Vulkan/RenderPipeline.h" 
#include "Vulkan/CommandBuffer.h"
#include <imgui/backends/imgui_impl_vulkan.h>
#endif

#include <GLFW/glfw3.h>

#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN) 
GLFWwindow* Window::GetRaw() {
	return window;
}

Window::Window(const std::string& name)
	: ResourceBase(name)
{
	window = nullptr;
	size = glm::ivec2(0, 0);

	/*std::function<void(GLFWwindow* window, int width, int height)> frameBufferSizeCallback = [&](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	};

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback.target<void(GLFWwindow * window, int width, int height)>());*/
	ResourceManager::addResource<Window>(this);
}

Window::Window(const std::string& name, GLFWwindow* window)
	: window(std::move(window)) 
	, ResourceBase(name)
{
	this->window = window;
	glfwGetWindowSize(this->window, &size.x, &size.y);

	ResourceManager::addResource<Window>(this);
}

Window::Window(const std::string& name, int width, int height) 
	: ResourceBase(name)
{
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	size.x = width; size.y = height;

	ResourceManager::addResource<Window>(this);
}

Window::~Window() {
	glfwDestroyWindow(window);
	window = nullptr;
}

void Window::SetWindow(GLFWwindow* window) {
	this->window = window;

	glfwGetWindowSize(this->window, &size.x, &size.y);
}

std::shared_ptr<Window> Window::CreateWindow(const std::string& name, const unsigned int& width, const unsigned int& height) {
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	size.x = width; size.y = height;
	return std::make_shared<Window>(*this);
}

void Window::Awake(uint32_t currentFrame) {

}

void Window::Start(uint32_t currentFrame) {
	

	//
}

void Window::Update(uint32_t currentFrame) {
	
}

void Window::FixedUpdate() {

}

std::shared_ptr<Panel>& Window::AddPanel(const std::shared_ptr<Panel>& ui) {
	return panels.emplace(ui->name, ui).first->second;
}

std::shared_ptr<Panel> Window::GetPanel(const std::string& name) const {
	auto element = panels.find(name);

	if (element != panels.end())
		return element->second;

	return nullptr;
}

void Window::RemovePanel(const std::string name) {
	if(panels.contains(name))
		panels.erase(name);
}
#endif