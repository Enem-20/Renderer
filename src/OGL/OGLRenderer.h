#pragma once

#ifndef OGL_RENDERER
#define OGL_RENDERER

#include "../BaseRenderer.h"

class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class BaseTexture2D;

class OGLRenderer : public BaseRenderer {
public:
	OGLRenderer(const std::string& name);
	~OGLRenderer();

	void render() override;
	void drawFrame() override;

	void initWindow() override;

	std::shared_ptr<BaseTexture2D> createTexture(std::string_view name, const std::string& relativePath, const uint32_t& texWidth, const uint32_t& texHeight, unsigned char* pixels, const uint32_t& texChannels = 4) override;
	void addTexture(std::shared_ptr<BaseTexture2D> texture) override;
	void removeTexture(std::string_view name) override;

	void OnBeforeFrame() override;

	void setViewport(int width = 1080, int height = 1080, int leftOffset = 0, int bottomOffset = 0) override;

	void afterComplete() override;

	GENERATETYPE(OGLRenderer)
private:
};

#endif // !OGL_RENDERER