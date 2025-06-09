#pragma once

#ifndef OGL_RENDERER_H
#define OGL_RENDERER_H

#include "../BaseRenderer.h"

class OGLRenderer : public BaseRenderer {
public:
	OGLRenderer(const std::string& name);
	~OGLRenderer();
	void render() override;
	void drawFrame() override;

	void initWindow() override;

	std::shared_ptr<Texture2D> createWindow(std::string_view name, const std::string& relativePath, int texWidth, int texHeight, int texChannels, unsigned char* pixels) {
		return nullptr;
	}

	bool windowShouldClose() const {
		return false;
	}

	void addTexture(std::shared_ptr<Texture2D> texture) override;
	void removeTexture(const std::string& name) override;

	void OnBeforeFrame() override;

	void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) override;

	GENERATETYPE(OGLRenderer)
private:
};

//BaseRenderer(const std::string& name) : ResourceBase(name), ViewportSize(glm::vec2(0,0)){}
//	virtual ~BaseRenderer(){}
//
//	virtual void render() = 0;
//	virtual void drawFrame() = 0;
//
//	virtual void initWindow() = 0;
//
//	virtual std::shared_ptr<Texture2D> createWindow(std::string_view name, const std::string& relativePath, int texWidth, int texHeight, int texChannels, unsigned char* pixels) = 0;
//
//	virtual void addTexture(std::shared_ptr<Texture2D> texture) = 0;
//	virtual void removeTexture(const std::string& name) = 0;
//
//	virtual void OnBeforeFrame()= 0;
//
//	virtual bool windowShouldClose() const;
//	virtual void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) = 0;

#endif // !OGL_RENDERER