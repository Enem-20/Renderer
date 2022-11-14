project(Renderer)

target_sources(Renderer INTERFACE
	src/Vulkan/PhysicalDevice.h
	src/Vulkan/PhysicalDevice.cpp
	src/Vulkan/LogicalDevice.h
	src/Vulkan/LogicalDevice.cpp)

add_library(Renderer INTERFACE)

add_Subdirectory(external/glfw)
add_subdirectory(external/VulkanHpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE 
		"$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
		)
endif(WIN32)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE 
		"$ENV{VULKAN_SDK}/Lib/vulkan-1.a"
		)
endif(UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	glfw
	)

target_include_directories(${PROJECT_NAME} PRIVATE 
	external/VulkanHpp/Vulkan-Headers/include
	external/glm
	external/stb
)