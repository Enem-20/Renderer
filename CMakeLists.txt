project(Renderer)

target_sources(Renderer INTERFACE
	src/Vulkan/PhysicalDevice.h
	src/Vulkan/PhysicalDevice.cpp
	src/Vulkan/LogicalDevice.h
	src/Vulkan/LogicalDevice.cpp)

add_library(Renderer INTERFACE)