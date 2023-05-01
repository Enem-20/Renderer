#include "WindowSurface.h"

#include "../Window.h"
#include "../WindowManager.h"

#include "Instance.h"

WindowSurface::WindowSurface(Instance& instance)
	: instance(instance)
{
	if (glfwCreateWindowSurface(instance.getRaw(), WindowManager::GetCurrentWindow().GetRaw(), nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
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