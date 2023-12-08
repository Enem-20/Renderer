#pragma once

#include "API/ExportPropety.h"

#include "../BaseShaderProgram.h"

#include <glm/mat4x4.hpp>

#include <string>

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

class DLLEXPORT OGLShaderProgram : BaseShaderProgram
{
public:
	OGLShaderProgram(std::string_view name, const std::string& vertexShader, const std::string& fragmentShader);
	~OGLShaderProgram();

	OGLShaderProgram() = delete;
	OGLShaderProgram(OGLShaderProgram&);
	OGLShaderProgram& operator=(const OGLShaderProgram&) = delete;
	OGLShaderProgram& operator=(OGLShaderProgram&& shaderProgram) noexcept;
	OGLShaderProgram(OGLShaderProgram&& shaderProgram) noexcept;

	bool isCompiled() const;
	void use() const;
	void setInt(const std::string& name, const GLint value);
	void setMatrix4(const std::string& name, const glm::mat4& matrix);

	GENERATETYPE(OGLShaderProgram)
private:
	bool createShaders(const std::string& vertexShader, const std::string& fragmentShader);
	bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

	bool m_isCompiled = false;
	GLuint m_ID = 0;
	GLuint* vertexShaderID;
	GLuint* fragmentShaderID;
};