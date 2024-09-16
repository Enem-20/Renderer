project(Renderer)

find_package(Vulkan REQUIRED)
find_library(Vulkan_LIBRARY NAMES vulkan-1 PATHS ${VULKAN_SDK}/Lib NO_DEFAULT_PATH)

add_library(${PROJECT_NAME} INTERFACE)

FILE(GLOB main_headers "src/*.h")
FILE(GLOB main_sources "src/*.cpp")
FILE(GLOB_RECURSE imgui_headers "src/ImGui/*.h")
FILE(GLOB_RECURSE imgui_sources "src/ImGui/*.cpp")
FILE(GLOB_RECURSE vulkan_headers "src/Vulkan/*.h")
FILE(GLOB_RECURSE vulkan_sources "src/Vulkan/*.cpp")
FILE(GLOB_RECURSE ogl_headers "src/OGL/*.h")
FILE(GLOB_RECURSE ogl_sources "src/OGL/*.cpp")

target_sources(Renderer INTERFACE
	${main_headers}
	${main_sources}
	${imgui_headers}
	${imgui_sources}
	${vulkan_headers}
	${vulkan_sources}
	${ogl_headers}
	${ogl_sources}
)

target_compile_definitions(Renderer INTERFACE GLM_FORCE_DEPTH_ZERO_TO_ONE GLFW_INCLUDE_VULKAN)

#target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

add_subdirectory(external/glfw)
#add_subdirectory(external/VulkanHpp)
add_subdirectory(external/glad)



set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

target_include_directories(Renderer INTERFACE
	${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(Renderer INTERFACE 
	${Vulkan_LIBRARY}
	glfw
	glad
)

