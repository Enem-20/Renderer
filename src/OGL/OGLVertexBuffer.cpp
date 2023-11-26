#include "OGLVertexBuffer.h"

#include "Resources/ResourceManager.h"

OGLVertexBuffer::OGLVertexBuffer(const std::string& name)
	: BaseRenderer(name)
	, m_id(0)
{
	ResourceManager::addResource<OGLVertexBuffer>(this);
}
OGLVertexBuffer::~OGLVertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

OGLVertexBuffer& OGLVertexBuffer::operator=(OGLVertexBuffer&& vertexBuffer) noexcept
{
	name = vertexBuffer.name;
	m_id = vertexBuffer.m_id;
	vertexBuffer.m_id = 0;
	return *this;
}
OGLVertexBuffer::OGLVertexBuffer(OGLVertexBuffer&& vertexBuffer) noexcept
	: BaseRenderer(vertexBuffer.name)
{
	m_id = vertexBuffer.m_id;
	vertexBuffer.m_id = 0;
}

void OGLVertexBuffer::init(const void* data, const unsigned int size, const int RenderMode)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, data, RenderMode);
}
void OGLVertexBuffer::update(const void* data, const unsigned int size) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
void OGLVertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}
void OGLVertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}