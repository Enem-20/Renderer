#pragma once

#ifndef RENDERER_MANAGER
#define RENDERER_MANAGER

#include <memory>
#include <string>

class BaseRenderer;

class BaseState {
public:
	BaseState(std::unique_ptr<BaseRenderer> newRenderer = nullptr);
	
	BaseRenderer& getRenderer() const;
	virtual void init() = 0;
	virtual void loadShaderProgram(std::string_view shaderName, const std::string& vertexPath, const std::string& fragmentPath) = 0;

	void setRenderer(std::unique_ptr<BaseRenderer> newRenderer);
	void pushShaderLoader(const std::function<void()>& shaderLoader);
	void applyShaderLoaders();
protected:
	std::unique_ptr<BaseRenderer> currentRenderer;
	std::queue<std::function<void()>> shaderLoaders;
};

class VulkanState : BaseState {
public:
	VulkanState();
	void init() override;
	void loadShaderProgram(std::string_view shaderName, const std::string& vertexPath, const std::string& fragmentPath) override;
};

class OGLState : BaseState {
public:
	OGLState();
	void init() override;
	void loadShaderProgram(std::string_view shaderName, const std::string& vertexPath, const std::string& fragmentPath) override;
};

class RendererManager {
public:
	static void setState(std::unique_ptr<BaseState> newState);
	static BaseRenderer& getRenderer();
	static void loadShaderProgram(std::string_view shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	static void applyShaderLoaders();
private:
	static std::unique_ptr<BaseState> state;
};

#endif