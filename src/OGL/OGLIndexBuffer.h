#pragma once

#ifndef OGL_INDEX_BUFFER_H
#define OGL_INDEX_BUFFER_H

typedef unsigned int GLuint;


class OGLIndexBuffer
{
public:
	OGLIndexBuffer();
	~OGLIndexBuffer();

	OGLIndexBuffer(const OGLIndexBuffer&) = delete;
	OGLIndexBuffer& operator=(const OGLIndexBuffer&) = delete;
	OGLIndexBuffer& operator=(OGLIndexBuffer&& indexBuffer) noexcept;
	OGLIndexBuffer(OGLIndexBuffer&& indexBuffer) noexcept;

	void init(const void* data, const unsigned int count);
	void bind() const;
	void unbind() const;
	unsigned int getCount() const { return m_count; }

private:
	GLuint m_id;
	unsigned int m_count;
};

#endif