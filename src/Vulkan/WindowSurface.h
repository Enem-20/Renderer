#pragma once

#include "Capturer.h"

#include "GLFW\glfw3.h"

#include <memory>

class Instance;

class WindowSurface{
public:
	WindowSurface(Instance& instance);
	~WindowSurface();

	void destroyWindowSurface();
	VkSurfaceKHR& getRaw();
	
	VkSurfaceKHR surface;
	Instance& instance;
};