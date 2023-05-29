project(Renderer)

set(RendererLibrary 
	src/Vulkan/WindowSurface.h
	src/Vulkan/WindowSurface.cpp
	src/Vulkan/Capturer.h
	src/Vulkan/Interface.h
	src/Vulkan/GeneralVulkanStorage.h
	src/Vulkan/GeneralVulkanStorage.cpp
	src/Vulkan/DebugMessenger.h
	src/Vulkan/DebugMessenger.cpp
	src/Vulkan/Instance.h
	src/Vulkan/Instance.cpp
	src/Vulkan/PhysicalDevice.h
	src/Vulkan/PhysicalDevice.cpp
	src/Vulkan/LogicalDevice.h
	src/Vulkan/LogicalDevice.cpp
	src/Vulkan/SwapChain.h
	src/Vulkan/SwapChain.cpp
	src/Vulkan/RenderPipeline.h
	src/Vulkan/RenderPipeline.cpp
	src/Vulkan/RenderPass.h
	src/Vulkan/RenderPass.cpp
	src/Vulkan/RenderPass.cpp
	src/Vulkan/Vertex.h
	src/Vulkan/Vertex.cpp
	src/Vulkan/CommandBuffer.h
	src/Vulkan/CommandBuffer.cpp
	src/Vulkan/SingleTimeBuffer.h
	src/Vulkan/SingleTimeBuffer.cpp
	src/Vulkan/CommandPool.h
	src/Vulkan/CommandPool.cpp
	src/Vulkan/ImageView.h
	src/Vulkan/ImageView.cpp
	src/Vulkan/VertexBuffer.h
	src/Vulkan/VertexBuffer.cpp
	src/Vulkan/IndexBuffer.h
	src/Vulkan/IndexBuffer.cpp
	src/Vulkan/UniformBuffer.h
	src/Vulkan/UniformBuffer.cpp
	src/Vulkan/DescriptorPool.h
	src/Vulkan/DescriptorPool.cpp
	src/Vulkan/SyncObjects.h
	src/Vulkan/SyncObjects.cpp
	src/Vulkan/DescriptionSets.h
	src/Vulkan/DescriptionSets.cpp
	src/Vulkan/DescriptorSetLayout.h
	src/Vulkan/DescriptorSetLayout.cpp
	src/Vulkan/DescriptorSetBase.h
	src/Vulkan/DescriptorSetBase.cpp
	src/Vulkan/DepthResources.h
	src/Vulkan/DepthResources.cpp
	src/Vulkan/ImageProcessing.h
	src/Vulkan/ImageProcessing.cpp
	src/Vulkan/ColorResources.h
	src/Vulkan/ColorResources.cpp
	src/ImGui/ImGui.h
	src/ImGui/ImGui.cpp
	src/ShaderProgram.h
	src/ShaderProgram.cpp
	src/DependencyOwner.h
	src/DependencyOwner.cpp
	src/Renderer.h
	src/Renderer.cpp
	src/Window.h
	src/Window.cpp
	src/WindowManager.h
	src/WindowManager.cpp
	src/Texture2D.h
	src/Texture2D.cpp
	src/Sprite.h
	src/Sprite.cpp
	src/UniformBufferObject.h
	src/UniformBufferObject.cpp
)

add_library(Renderer INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE
	${RendererLibrary}
)

target_compile_definitions(Renderer INTERFACE GLFW_INCLUDE_VULKAN)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

add_Subdirectory(external/glfw)
add_subdirectory(external/VulkanHpp)

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
)

target_include_directories(${PROJECT_NAME} INTERFACE 
	external/VulkanHpp/Vulkan-Headers/include
	external/glm
	external/stb
)