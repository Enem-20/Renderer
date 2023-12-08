#include "WindowSurface.h"

#include "../Window.h"
#include "../WindowManager.h"

#include "Instance.h"
#include "Resources/ResourceManager.h"

#include "GLFW/glfw3.h"

WindowSurface::WindowSurface(const std::string& name, Instance& instance)
	: instance(instance)
	, ResourceBase(name)
{
	if (glfwCreateWindowSurface(instance.getRaw(), WindowManager::GetCurrentWindow()->GetRaw(), nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	ResourceManager::addResource<WindowSurface>(this);
}

WindowSurface::~WindowSurface() {
	destroyWindowSurface();
}

void WindowSurface::destroyWindowSurface() {
	vkDestroySurfaceKHR(instance.getRaw(), surface, nullptr);
}

VkSurfaceKHR& WindowSurface::getRaw() {
	return surface;
}