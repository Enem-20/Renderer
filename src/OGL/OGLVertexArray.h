#pragma once

#include <glad/glad.h>

class OGLVertexBuffer;
class OGLVertexBufferLayout;

class OGLVertexArray
{
public:
	OGLVertexArray();
	~OGLVertexArray();

	OGLVertexArray(const OGLVertexArray&) = delete;
	OGLVertexArray& operator=(const OGLVertexArray&) = delete;
	OGLVertexArray& operator=(OGLVertexArray&& vertexArray) noexcept;
	OGLVertexArray(OGLVertexArray&& vertexArray) noexcept;

	void addBuffer(const OGLVertexBuffer& vertexBuffer, const OGLVertexBufferLayout& layout);
	void bind() const;
	void unbind() const;
private:
	GLuint m_id = 0;
	unsigned int m_ElementsCount = 0;
};