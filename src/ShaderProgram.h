#pragma once

#ifndef SHADER_РPROGRAM_H
#define SHADER_РPROGRAM_H
#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"

#include <glm/mat4x4.hpp>

#include <string>

struct VkShaderModule_T;
typedef VkShaderModule_T* VkShaderModule;

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

class DLLEXPORT ShaderProgram : public ResourceBase
{
#ifdef OGL
public:
	ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
	~ShaderProgram();

	ShaderProgram() = delete;
	ShaderProgram(ShaderProgram&);
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
	ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	bool isCompiled() const;
	void use() const;
	void setInt(const std::string& name, const GLint value);
	void setMatrix4(const std::string& name, const glm::mat4& matrix);
private:
	bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
	bool m_isCompiled = false;
	GLuint m_ID = 0;
#endif // !OGL
#ifdef GLFW_INCLUDE_VULKAN
public:
	ShaderProgram(std::string_view name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& vertexShader, const std::string& fragmentShader);
	~ShaderProgram();

	ShaderProgram() = delete;
	//ShaderProgram(ShaderProgram&);
	//ShaderProgram& operator=(const ShaderProgram&) = delete;
	//ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
	//ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	//bool isCompiled() const;
	//void use() const;
	//void setInt(const std::string& name, const GLint value);
	void setMatrix4(const std::string& name, const glm::mat4& matrix);
	VkShaderModule& getVertexShaderModule();
	VkShaderModule& getFragmentShaderModule();

	std::string vertexShaderPath;
	std::string fragmentShaderPath;
	GENERATETYPE(ShaderProgram)
private:
	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;
	VkShaderModule createShader(const std::string& source);
#endif // !GLFW_INCLUDE_VULKAN
};

#endif // !SHADERPROGRAM