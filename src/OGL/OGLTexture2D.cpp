#include "OGLTexture2D.h"

#include "Resources/ResourceManager.h"

#include <glad/glad.h>

OGLTexture2D::OGLTexture2D(std::string_view name, const std::string& relativePath, const uint32_t& width, const uint32_t& height,
	const unsigned char* data, const uint32_t& channels, const std::unordered_map<size_t, std::string_view>& dependencies_names,
	const GLenum filter, const GLenum wrapMode)
	: BaseTexture2D(name, relativePath, width, height, channels)
	, m_ID(0)
{
	switch (channels)
	{
	case 4:
		m_mode = GL_RGBA;
		break;
	case 3:
		m_mode = GL_RGB;
		break;
	case 1:
		m_mode = GL_RED;
		break;
	default:
		m_mode = GL_RGBA;
		break;
	}

	glGenTextures(1, &m_ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_mode, static_cast<int>(m_width), static_cast<int>(m_height), 0, m_mode, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	ResourceManager::addResource<OGLTexture2D>(this);
}

OGLTexture2D& OGLTexture2D::operator=(OGLTexture2D&& texture2d) noexcept
{
	glDeleteTextures(1, &m_ID);

	m_ID = texture2d.m_ID;
	texture2d.m_ID = 0;

	name = texture2d.name;
	m_mode = texture2d.m_mode;
	m_width = texture2d.m_width;
	m_height = texture2d.m_height;
	m_path = texture2d.m_path;

	return *this;
}

OGLTexture2D::OGLTexture2D(OGLTexture2D& texture2d)
	: BaseTexture2D(texture2d.name, texture2d.m_path, texture2d.m_width, texture2d.m_height, texture2d.channels)
{
	//glDeleteTextures(1, &m_ID);
	m_ID = texture2d.m_ID;
	texture2d.m_ID = 0;

	m_subTextures = texture2d.m_subTextures;
}

OGLTexture2D::OGLTexture2D(OGLTexture2D&& texture2d) noexcept
	: m_ID(std::move(texture2d.m_ID))
	, m_mode(std::move(texture2d.m_mode))
    , BaseTexture2D(texture2d.name, texture2d.m_path, texture2d.m_width, texture2d.m_height, texture2d.channels)
{
	texture2d.m_ID = 0;
}
OGLTexture2D::~OGLTexture2D()
{
	glDeleteTextures(1, &m_ID);
}

void OGLTexture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}