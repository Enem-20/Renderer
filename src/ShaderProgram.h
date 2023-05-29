#pragma once

#ifndef SHADERPROGRAM
#define SHADERPROGRAM
#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#ifdef SHOWONBUILD
#include "GLFW/glfw3.h"
#include <glm/mat4x4.hpp>
#else
//namespace glm {
//	struct mat4;
//}

struct VkShaderModule;
#endif

#include <string>

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
	ShaderProgram(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& vertexShader, const std::string& fragmentShader);
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
#ifdef SHOWONBUILD
private:
	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;
	VkShaderModule createShader(const std::string& source);
#endif
#endif // !GLFW_INCLUDE_VULKAN
};

#endif // !SHADERPROGRAM