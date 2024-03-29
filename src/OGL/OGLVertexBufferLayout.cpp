#include "OGLVertexBufferLayout.h"


OGLVertexBufferLayout::OGLVertexBufferLayout()
	: m_stride(0)
{

}

void OGLVertexBufferLayout::reserveElements(const size_t count)
{
	m_LayoutElements.reserve(count);
}
void OGLVertexBufferLayout::addElementLayoutFloat(const unsigned int count, const bool normalized)
{
	m_LayoutElements.push_back({ static_cast<GLint>(count), GL_FLOAT, normalized });
	m_stride += m_LayoutElements.back().size;
}