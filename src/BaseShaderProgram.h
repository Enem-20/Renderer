#pragma once

#ifndef BASE_SHADERPROGRAM
#define BASE_SHADERPROGRAM

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"

#include <glm/mat4x4.hpp>

#include <string>

struct VkShaderModule_T;
typedef VkShaderModule_T* VkShaderModule;

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

class DLLEXPORT BaseShaderProgram : public ResourceBase
{
#ifdef OGL
public:
	ShaderProgram(std::string_view vertexShader, std::string_view fragmentShader);
	~ShaderProgram();

	ShaderProgram() = delete;
	ShaderProgram(ShaderProgram&);
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
	ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

	bool isCompiled() const;
	void use() const;
	void setInt(std::string_view name, const GLint value);
	void setMatrix4(std::string_view name, const glm::mat4& matrix);
private:
	bool createShader(std::string_view source, const GLenum shaderType, GLuint& shaderID);
	bool m_isCompiled = false;
	GLuint m_ID = 0;
#endif // !OGL
#ifdef GLFW_INCLUDE_VULKAN
public:
	BaseShaderProgram(std::string_view name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~BaseShaderProgram();
	BaseShaderProgram(const BaseShaderProgram& shaderProgram);
	BaseShaderProgram(BaseShaderProgram&& shaderProgram) noexcept;

	BaseShaderProgram() = delete;

	std::string vertexShaderPath;
	std::string fragmentShaderPath;

	GENERATETYPE(BaseShaderProgram)
#endif // !GLFW_INCLUDE_VULKAN
};

#endif