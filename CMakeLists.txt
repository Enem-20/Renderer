project(Renderer)

add_library(${PROJECT_NAME} INTERFACE)

FILE(GLOB main_headers "src/*.h")
FILE(GLOB main_sources "src/*.cpp")
FILE(GLOB_RECURSE imgui_headers "src/ImGui/*.h")
FILE(GLOB_RECURSE imgui_sources "src/ImGui/*.cpp")
FILE(GLOB_RECURSE vulkan_headers "src/Vulkan/*.h")
FILE(GLOB_RECURSE vulkan_sources "src/Vulkan/*.cpp")
FILE(GLOB_RECURSE ogl_headers "src/OGL/*.h")
FILE(GLOB_RECURSE ogl_sources "src/OGL/*.cpp")

target_sources(${PROJECT_NAME} INTERFACE
	${main_headers}
	${main_sources}
	${imgui_headers}
	${imgui_sources}
	${vulkan_headers}
	${vulkan_sources}
	${ogl_headers}
	${ogl_sources}
)

target_compile_definitions(Renderer INTERFACE GLFW_INCLUDE_VULKAN)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

add_subdirectory(external/glfw)
add_subdirectory(external/VulkanHpp)
add_subdirectory(external/glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} INTERFACE 
		"$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
		)
endif(WIN32)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} INTERFACE 
		"$ENV{VULKAN_SDK}/Lib/vulkan-1.a"
	)
endif(UNIX)

target_link_libraries(${PROJECT_NAME} INTERFACE 
	glfw
	glad
)

target_include_directories(${PROJECT_NAME} INTERFACE 
	external/VulkanHpp/Vulkan-Headers/include
	external/glm
	external/stb
)