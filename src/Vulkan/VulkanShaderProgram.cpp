#include "VulkanShaderProgram.h"

#include "RenderPipeline.h"
#include "LogicalDevice.h"
#include "../RendererManager.h"

#include "Resources/ResourceManager.h"

#include "Tools/casts.h"
#include "Logging/Clerk.h"


#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

VulkanShaderProgram::VulkanShaderProgram(std::string_view name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& vertexShader, const std::string& fragmentShader)
	: BaseShaderProgram(name, vertexShaderPath, fragmentShaderPath)
{
	vertexShaderModule = createShader(vertexShader);
	fragmentShaderModule = createShader(fragmentShader);

	ResourceManager::addResource<BaseShaderProgram>(this);
}

VulkanShaderProgram::~VulkanShaderProgram() {
}

VkShaderModule& VulkanShaderProgram::getVertexShaderModule() {
	return vertexShaderModule;
}

VkShaderModule& VulkanShaderProgram::getFragmentShaderModule() {
	return fragmentShaderModule;
}

bool VulkanShaderProgram::createShaders(const std::string& vertexShader, const std::string& fragmentShader) {
	createShader(vertexShader);
	createShader(fragmentShader);
	return true;
}

VkShaderModule VulkanShaderProgram::createShader(const std::string& source)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = source.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

	VkShaderModule shaderModule;
	auto renderer = static_cast<VulkanRenderer*>(&RendererManager::getRenderer());
	if (vkCreateShaderModule(renderer->getLogicalDevice()->getRaw(), &createInfo, nullptr, &shaderModule)) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}