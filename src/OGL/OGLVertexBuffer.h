#pragma once

#ifndef OGL_VERTEX_BUFFER_H
#define OGL_VERTEX_BUFFER_H

#include "../BaseRenderer.h"

#include <glad/glad.h>


class OGLVertexBuffer : public BaseRenderer
{
public:
	OGLVertexBuffer(const std::string& name);
	~OGLVertexBuffer();

	OGLVertexBuffer(const OGLVertexBuffer&) = delete;
	OGLVertexBuffer& operator=(const OGLVertexBuffer&) = delete;
	OGLVertexBuffer& operator=(OGLVertexBuffer&& vertexBuffer) noexcept;
	OGLVertexBuffer(OGLVertexBuffer&& vertexBuffer) noexcept;

	void init(const void* data, const unsigned int size, const int RenderMode = GL_DYNAMIC_DRAW);
	void update(const void* data, const unsigned int size) const;
	void bind() const;
	void unbind() const;

private:
	GLuint m_id;
};

#endif