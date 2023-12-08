#include "ShaderProgram.h"

#ifdef OGL
#include <glad/glad.h>
#endif //!OGL

#ifdef GLFW_INCLUDE_VULKAN
#include "Vulkan/RenderPipeline.h"
#include "Vulkan/LogicalDevice.h"
#endif

#include "Resources/ResourceManager.h"

#include "Tools/casts.h"
#include "Logging/Clerk.h"


#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#ifdef GLFW_INCLUDE_VULKAN

ShaderProgram::ShaderProgram(std::string_view name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& vertexShader, const std::string& fragmentShader)
	: ResourceBase(name)
	, vertexShaderPath(vertexShaderPath)
	, fragmentShaderPath(fragmentShaderPath)
{
	vertexShaderModule = createShader(vertexShader);
	fragmentShaderModule = createShader(fragmentShader);

	ResourceManager::addResource<ShaderProgram>(this);
}

ShaderProgram::~ShaderProgram() {
}

VkShaderModule ShaderProgram::createShader(const std::string& source)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = source.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(ResourceManager::getResource<LogicalDevice>("TestLogicalDevice")->getRaw(), &createInfo, nullptr, &shaderModule)) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

#endif

#ifdef OGL
void ShaderProgram::use() const
{
	glUseProgram(m_ID);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
{
	glDeleteProgram(m_ID);
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;

	return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram& shaderProgram)
{
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
{
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
}

void ShaderProgram::setInt(std::string_view name, const GLint value)
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::setMatrix4(std::string_view name, const glm::mat4& matrix)
{

	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

bool ShaderProgram::isCompiled() const { return m_isCompiled; }
#endif

VkShaderModule& ShaderProgram::getVertexShaderModule() {
	return vertexShaderModule;
}

VkShaderModule& ShaderProgram::getFragmentShaderModule() {
	return fragmentShaderModule;
}