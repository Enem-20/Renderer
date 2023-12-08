#include "RendererManager.h"

#include "OGL/OGLRenderer.h"
#include "OGL/OGLShaderProgram.h"

#include "Vulkan/VulkanShaderProgram.h"
#include "Vulkan/VulkanRenderer.h"

#include "BaseRenderer.h"

#include "Resources/ResourceManager.h"

#include <iostream>

BaseState::BaseState(std::unique_ptr<BaseRenderer> newRenderer) 
	: currentRenderer(std::move(newRenderer))
{}

void BaseState::setRenderer(std::unique_ptr<BaseRenderer> newRenderer) {
	currentRenderer = std::move(newRenderer);
}

BaseRenderer& BaseState::getRenderer() const {
	if (!currentRenderer)
		std::runtime_error("you couldn't get rendere because it didn't created");
	
	return *currentRenderer;
}

void BaseState::pushShaderLoader(const std::function<void()>& shaderLoader) {
	shaderLoaders.push(shaderLoader);
}

void BaseState::applyShaderLoaders() {
	while (!shaderLoaders.empty()) {
		shaderLoaders.front()();
		shaderLoaders.pop();
	}
}

VulkanState::VulkanState() 
{}

void VulkanState::init() {
	setRenderer(std::make_unique<VulkanRenderer>());
}

void VulkanState::loadShaderProgram(std::string_view shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	pushShaderLoader([shaderName, vertexPath, fragmentPath]() {ResourceManager::loadShaderProgram<VulkanShaderProgram>(shaderName, vertexPath, fragmentPath); });
}


OGLState::OGLState()
{}

void OGLState::init() {
	setRenderer(std::make_unique<OGLRenderer>());
}

void OGLState::loadShaderProgram(std::string_view shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	pushShaderLoader([shaderName, vertexPath, fragmentPath]() {ResourceManager::loadShaderProgram<OGLShaderProgram>(shaderName, vertexPath, fragmentPath); });
}

void RendererManager::setState(std::unique_ptr<BaseState> newState) {
	state = std::move(newState);
}

BaseRenderer& RendererManager::getRenderer() {
	return state->getRenderer();
}

void RendererManager::loadShaderProgram(std::string_view shaderName, const std::string& vertexPath, const std::string& fragmentPath) {
	state->loadShaderProgram(shaderName, vertexPath, fragmentPath);
}

void RendererManager::applyShaderLoaders() {
	state->applyShaderLoaders();
}