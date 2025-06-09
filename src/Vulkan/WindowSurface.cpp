#include "WindowSurface.h"

#include <GLFW/glfw3.h>

#include "../Window.h"
#include "../WindowManager.h"

#include "Instance.h"
#include "Device.h"
#include "SwapChain.h"


WindowSurface::WindowSurface(std::string_view name, Instance* instance)
	: ResourceBase(name)
	, instance(instance)
{
	if (glfwCreateWindowSurface(instance->getRaw(), WindowManager::GetCurrentWindow()->GetRaw(), nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

WindowSurface::~WindowSurface() {
	destroyWindowSurface();
}

void WindowSurface::addDevice(std::string_view name, Device* device) {
	if(device && !supportedDevices.contains(name.data())) {
		supportedDevices[name.data()] = device;
	}
}

void WindowSurface::removeDevice(std::string_view name) {
	if(supportedDevices.contains(name.data())) {
		supportedDevices.erase(name.data());
	}
}

Device* WindowSurface::getSupportedDevice(std::string_view name) const {
	if(supportedDevices.contains(name.data())) {
		return supportedDevices.at(name.data());
	}
	return nullptr;
}

tsl::hopscotch_map<std::string, Device*>& WindowSurface::getSupportedDevices() {
	return supportedDevices;
}

void WindowSurface::destroyWindowSurface() {
	vkDestroySurfaceKHR(instance->getRaw(), surface, nullptr);
}

VkSurfaceKHR& WindowSurface::getRaw() {
	return surface;
}

SwapChain* WindowSurface::getSwapChain(std::string_view name) const {
	if(swapChains.contains(name.data())) {
		return swapChains.at(name.data());
	}
	return nullptr;
}

bool WindowSurface::getFramebufferResized() const {
	return framebufferResized;
}

void WindowSurface::setFramebufferResized(bool resized) {
	framebufferResized = resized;
}