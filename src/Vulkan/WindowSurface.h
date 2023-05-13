#pragma once

#include "../../src/Resources/ResourceBase.h"

#include "GLFW\glfw3.h"

#include <memory>

class Instance;

class WindowSurface : public ResourceBase{
public:
	WindowSurface() = default;
	WindowSurface(const std::string& name, Instance& instance);
	~WindowSurface();

	void destroyWindowSurface();
	VkSurfaceKHR& getRaw();
	
	inline static const std::string type = GETTYPE(WindowSurface);
private:
	VkSurfaceKHR surface;
	Instance& instance;

};