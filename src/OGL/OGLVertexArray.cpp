#include "OGLVertexArray.h"

#include "OGLVertexBuffer.h"
#include "OGLVertexBufferLayout.h"

OGLVertexArray::OGLVertexArray()
{
	glGenVertexArrays(1, &m_id);
}
OGLVertexArray::~OGLVertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

OGLVertexArray& OGLVertexArray::operator=(OGLVertexArray&& vertexArray) noexcept
{
	m_id = vertexArray.m_id;
	vertexArray.m_id = 0;
	return *this;
}
OGLVertexArray::OGLVertexArray(OGLVertexArray&& vertexArray) noexcept
{
	m_id = vertexArray.m_id;
	vertexArray.m_id = 0;
}

void OGLVertexArray::bind() const
{
	glBindVertexArray(m_id);
}
void OGLVertexArray::unbind() const
{
	glBindVertexArray(0);
}

void OGLVertexArray::addBuffer(const OGLVertexBuffer& vertexBuffer, const OGLVertexBufferLayout& layout)
{
	bind();
	vertexBuffer.bind();
	const auto& layoutElements = layout.getLayoutElements();
	GLbyte* offset = nullptr;
	for (unsigned int i = 0; i < layoutElements.size(); ++i)
	{
		const auto& currentLayoutElement = layoutElements[i];
		GLuint currentAttribIndex = m_ElementsCount + i;
		glEnableVertexAttribArray(m_ElementsCount);
		glVertexAttribPointer(currentAttribIndex, currentLayoutElement.count, currentLayoutElement.type, currentLayoutElement.normalized, layout.getStride(), offset);
		offset += currentLayoutElement.size;
	}
	m_ElementsCount += static_cast<unsigned int>(layoutElements.size());
}