#pragma once

#ifndef WINDOW_SURFACE_H
#define WINDOW_SURFACE_H

#include "Resources/ResourceBase.h"

#include <tsl/hopscotch_map.h>



struct VkSurfaceKHR_T;
typedef VkSurfaceKHR_T* VkSurfaceKHR;

class SwapChain;

class Instance;
class Device;

class WindowSurface : public ResourceBase{
public:
	WindowSurface(std::string_view name, Instance* instance);
	~WindowSurface();

	void addDevice(std::string_view name, Device* device);
	void removeDevice(std::string_view name);
	Device* getSupportedDevice(std::string_view name) const;
	tsl::hopscotch_map<std::string, Device*>& getSupportedDevices();

	void destroyWindowSurface();

	VkSurfaceKHR& getRaw();
	SwapChain* getSwapChain(std::string_view name) const;
	bool getFramebufferResized() const;
	void setFramebufferResized(bool resized);

	GENERATETYPE(WindowSurface)
private:
	VkSurfaceKHR surface;
	Instance* instance;
	bool framebufferResized = false;

	tsl::hopscotch_map<std::string, Device*> supportedDevices;
	tsl::hopscotch_map<std::string, SwapChain*> swapChains;
};

#endif