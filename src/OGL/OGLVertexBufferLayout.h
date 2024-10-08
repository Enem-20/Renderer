#pragma once

#ifndef VERTEX_BUFFER_LAYOUT_ELEMENT_H
#define VERTEX_BUFFER_LAYOUT_ELEMENT_H

#include <vector>

#include <glad/glad.h>


struct VertexBufferLayoutElement
{
	GLint count;
	GLenum type;
	GLboolean normalized;
	unsigned int size;
};

class OGLVertexBufferLayout
{
public:
	OGLVertexBufferLayout();

	void reserveElements(const size_t count);
	unsigned int getStride() const { return m_stride; }
	void addElementLayoutFloat(const unsigned int count, const bool normalized);
	const std::vector<VertexBufferLayoutElement>& getLayoutElements() const { return m_LayoutElements; }
private:
	std::vector<VertexBufferLayoutElement> m_LayoutElements;
	unsigned int m_stride;
};

#endif