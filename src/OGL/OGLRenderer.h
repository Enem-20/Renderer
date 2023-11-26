#pragma once

#ifndef OGL_RENDERER
#define OGL_RENDERER

#include "../BaseRenderer.h"

class OGLRenderer : public BaseRenderer {
public:
	OGLRenderer(const std::string& name);
	~OGLRenderer();
	void render() override;
	void drawFrame() override;

	void initWindow() override;

	void addTexture(std::shared_ptr<Texture2D> texture) override;
	void removeTexture(const std::string& name) override;

	void OnBeforeFrame() override;

	void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) override;

	GENERATETYPE(OGLRenderer)
private:
};

#endif // !OGL_RENDERER