#include "OGLShaderProgram.h"


#include "Resources/ResourceManager.h"

#include "Tools/casts.h"

#include "Logging/Clerk.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <iostream>

OGLShaderProgram::OGLShaderProgram(std::string_view name, const std::string& vertexShader, const std::string& fragmentShader)
	: BaseShaderProgram(name, vertexShader, fragmentShader)
{
	createShaders(vertexShader, fragmentShader);

	glAttachShader(m_ID, *vertexShaderID);
	glAttachShader(m_ID, *fragmentShaderID);
	glLinkProgram(m_ID);

	GLint success;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLchar infolog[1024];
		glGetShaderInfoLog(m_ID, 1024, nullptr, infolog);
#ifdef LOG_ERR
		Clerk::Misstep(__LINE__, __FILE__, "OGLShaderProgram::OGLShaderProgram", Casts::StringToWstring(std::string("ERROR::SHADER: Link time error: ") + std::string(infolog)));
#endif
	}
	else
	{
		m_isCompiled = true;
	}

	glDeleteShader(*vertexShaderID);
	glDeleteShader(*fragmentShaderID);
	delete vertexShaderID;
	delete fragmentShaderID;
	ResourceManager::addResource<BaseShaderProgram>(this);
}
OGLShaderProgram::~OGLShaderProgram()
{
	glDeleteProgram(m_ID);
}

bool OGLShaderProgram::createShaders(const std::string& vertexShader, const std::string& fragmentShader) {
	if (!createShader(vertexShader, GL_VERTEX_SHADER, *vertexShaderID))
	{
#ifdef LOG_ERR
		Clerk::Misstep(__LINE__, __FILE__, "OGLShaderProgram::OGLShaderProgram", L"VERTEX SHADER compile time error");
#endif
		return;
}

	if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, *fragmentShaderID))
	{
#ifdef LOG_ERR
		Clerk::Misstep(__LINE__, __FILE__, "OGLShaderProgram::OGLShaderProgram", L"FRAGMENT SHADER compile time error");
#endif
		glDeleteShader(*vertexShaderID);
		return;
	}

	m_ID = glCreateProgram();
}

bool OGLShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
{
	shaderID = glCreateShader(shaderType);
	const char* code = source.c_str();
	glShaderSource(shaderID, 1, &code, nullptr);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infolog[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, infolog);
		Clerk::Misstep(__LINE__, __FILE__, "OGLShaderProgram::OGLShaderProgram", Casts::StringToWstring(std::string("ERROR::SHADER: Compile time error: ") + std::string(infolog)));
		std::cerr << "ERROR::SHADER: Compile time error:\n" << infolog << std::endl;
		system("pause");
		return false;
	}
	return true;
}

void OGLShaderProgram::use() const
{
	glUseProgram(m_ID);
}

OGLShaderProgram& OGLShaderProgram::operator=(OGLShaderProgram&& shaderProgram) noexcept
{
	glDeleteProgram(m_ID);
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;

	return *this;
}

OGLShaderProgram::OGLShaderProgram(OGLShaderProgram& shaderProgram)
	: BaseShaderProgram(shaderProgram)
{
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
}

OGLShaderProgram::OGLShaderProgram(OGLShaderProgram&& shaderProgram) noexcept
	: BaseShaderProgram(shaderProgram)
{
	m_ID = shaderProgram.m_ID;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_ID = 0;
	shaderProgram.m_isCompiled = false;
}

void OGLShaderProgram::setInt(const std::string& name, const GLint value)
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void OGLShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix)
{

	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

bool OGLShaderProgram::isCompiled() const { return m_isCompiled; }