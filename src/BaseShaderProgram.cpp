#include "BaseShaderProgram.h"

BaseShaderProgram::BaseShaderProgram(std::string_view name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) 
	: ResourceBase(name)
	, vertexShaderPath(vertexShaderPath)
	, fragmentShaderPath(fragmentShaderPath)
{}

BaseShaderProgram::~BaseShaderProgram() {

}

BaseShaderProgram::BaseShaderProgram(const BaseShaderProgram& shaderProgram) 
	: ResourceBase(shaderProgram.name)
	, vertexShaderPath(shaderProgram.vertexShaderPath)
	, fragmentShaderPath(shaderProgram.fragmentShaderPath)
{}

BaseShaderProgram::BaseShaderProgram(BaseShaderProgram&& shaderProgram) noexcept 
	: ResourceBase(std::move(shaderProgram.name))
	, vertexShaderPath(std::move(shaderProgram.vertexShaderPath))
	, fragmentShaderPath(std::move(shaderProgram.fragmentShaderPath))
{}