#include "Window.h"

//#include "../../UI/src/UIelement.h"
#include "../../internal/UI/src/UIelement.h"

#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN) 
#include <GLFW/glfw3.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#endif

#ifdef OGL
#include <imgui/backends/imgui_impl_opengl3.h>
#endif

#ifdef GLFW_INCLUDE_VULKAN
//#include <imgui/backends/imgui_impl_vulkan.h>
#endif

#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN) 
GLFWwindow* Window::GetRaw() {
	return window;
}

Window::Window() {
	window = nullptr;
	size = glm::ivec2(0, 0);
}

Window::Window(GLFWwindow* window)
	:window(std::move(window)) {
	this->window = window;
	glfwGetWindowSize(this->window, &size.x, &size.y);
}

Window::Window(std::string name, int width, int height) {
	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	size.x = width; size.y = height;
}

Window::~Window() {
	glfwDestroyWindow(window);
	window = nullptr;
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

void Window::Awake() {

}

void Window::Start() {
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize.x = static_cast<float>(size.x);
	io.DisplaySize.y = static_cast<float>(size.y);

#ifdef GLFW_INCLUDE_VULKAN
	ImGui_ImplVulkan_NewFrame();
#elif OGL
	ImGui_ImplOpenGL3_NewFrame();
#endif

	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
	for (auto& it : UIs)
		it.second->Start();

	ImGui::Render();

#ifdef GLFW_INCLUDE_VULKAN
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData());
#elif OGL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	

	glfwSwapBuffers(window);
}

void Window::Update() {
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize.x = static_cast<float>(size.x);
	io.DisplaySize.y = static_cast<float>(size.y);

#ifdef GLFW_INCLUDE_VULKAN
	ImGui_ImplVulkan_NewFrame();
#elif OGL
	ImGui_ImplOpenGL3_NewFrame();
#endif
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto& it : UIs)
		it.second->Update();

	ImGui::Render();

#ifdef GLFW_INCLUDE_VULKAN
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData()); //Paste command buffer here
#elif OGL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

	glfwSwapBuffers(window);
}

void Window::FixedUpdate() {

}

std::shared_ptr<UIelement>& Window::AddUI(const std::shared_ptr<UIelement>& ui) {
	std::shared_ptr<UIelement> _ui = std::make_shared<UIelement>(ui);
	UIs.emplace(ui->name, _ui);
	return _ui;
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