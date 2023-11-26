#include "OGLIndexBuffer.h"

#include <glad/glad.h>


OGLIndexBuffer::OGLIndexBuffer()
	: m_id(0)
	, m_count(0)
{

}

OGLIndexBuffer::~OGLIndexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

OGLIndexBuffer& OGLIndexBuffer::operator=(OGLIndexBuffer&& indexBuffer) noexcept
{
	m_id = indexBuffer.m_id;
	indexBuffer.m_id = 0;
	m_count = indexBuffer.m_count;
	indexBuffer.m_count = 0;
	return *this;
}

OGLIndexBuffer::OGLIndexBuffer(OGLIndexBuffer&& indexBuffer) noexcept
{
	m_id = indexBuffer.m_id;
	indexBuffer.m_id = 0;
	m_count = indexBuffer.m_count;
	indexBuffer.m_count = 0;
}

void OGLIndexBuffer::init(const void* data, const unsigned int count)
{
	m_count = count;
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

void OGLIndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void OGLIndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}