#pragma once

#ifndef OGL_TEXTURE2D
#define OGL_TEXTURE2D

#include "../BaseTexture2D.h"

#include <string>
#include <unordered_map>

typedef unsigned int GLenum;
typedef unsigned int GLuint;
#define GL_LINEAR 0x2601
#define GL_CLAMP_TO_EDGE 0x812F

class OGLTexture2D : public BaseTexture2D{
public:
	OGLTexture2D(std::string_view name, const std::string& relativePath, const uint32_t& width, const uint32_t& height,
		const unsigned char* data, const uint32_t& channels, const std::unordered_map<size_t, std::string_view>& dependencies_names,
		const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
	//Texture2D() = default;
	OGLTexture2D(OGLTexture2D& texture2d);
	OGLTexture2D& operator=(OGLTexture2D&& texture2d) noexcept;
	OGLTexture2D(OGLTexture2D&& texture2d) noexcept;
	~OGLTexture2D();

	void bind() override;

	GENERATETYPE(OGLTexture2D)
private:
	GLuint m_ID;
	GLenum m_mode;
};

#endif