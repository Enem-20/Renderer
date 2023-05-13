#include "Window.h"

//#include "../../UI/src/UIelement.h"
#include "../../UI/src/UIelement.h"


#include "../../src/Resources/ResourceManager.h"
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

#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN) 
GLFWwindow* Window::GetRaw() {
	return window;
}

Window::Window(const std::string& name)
	: ResourceBase(name)
{
	window = nullptr;
	size = glm::ivec2(0, 0);

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

	ResourceManager::removeResource<Window>(name);
	//glfwTerminate();
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
	

	//ImGui::ShowDemoWindow();
}

void Window::Start(uint32_t currentFrame) {
	

	//
}

void Window::Update(uint32_t currentFrame) {

	//
}

void Window::FixedUpdate() {

}

std::shared_ptr<UIelement>& Window::AddUI(const std::shared_ptr<UIelement>& ui) {
	return UIs.emplace(ui->name, ui).first->second;
}

std::shared_ptr<UIelement> Window::GetUI(const std::string& name) const {
	auto element = UIs.find(name);

	if (element != UIs.end())
		return element->second;

	return nullptr;
}

void Window::RemoveUI(const std::string name) {
	if(UIs.contains(name))
		UIs.erase(name);
}
#endif