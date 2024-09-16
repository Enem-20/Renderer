#pragma once

#ifndef WINDOW_SURFACE_H
#define WINDOW_SURFACE_H

#include "Resources/ResourceBase.h"

#include <memory>

class Instance;

struct VkSurfaceKHR_T;
typedef VkSurfaceKHR_T* VkSurfaceKHR;

class WindowSurface : public ResourceBase{
public:
	WindowSurface(const std::string& name, Instance& instance);
	~WindowSurface();

	void destroyWindowSurface();
	VkSurfaceKHR& getRaw();
	
	GENERATETYPE(WindowSurface)
private:
	VkSurfaceKHR surface;
	Instance& instance;

};

#endif