#include "BaseTexture2D.h"

//template<class DerivedTexture2DType>
BaseTexture2D/*<DerivedTexture2DType>*/::BaseTexture2D(std::string_view name, /*DerivedTexture2DType* derivedObject,*/ const std::string& relativePath, const uint32_t& texWidth, const uint32_t& texHeight, const uint32_t& channels)
	: ResourceBase(name/*, derivedObject*/)
	, m_width(texWidth)
	, m_height(texHeight)
	, m_path(relativePath)
	, channels(channels)
{}

//template<class DerivedTexture2DType>
BaseTexture2D/*<DerivedTexture2DType>*/::BaseTexture2D(const BaseTexture2D& texture2D)
	: ResourceBase(texture2D.name)
	, m_width(texture2D.m_width)
	, m_height(texture2D.m_width)
	, m_path(texture2D.m_path)
	, channels(texture2D.channels)
	, m_subTextures(texture2D.m_subTextures)
{}

//template<class DerivedTexture2DType>
void BaseTexture2D/*<DerivedTexture2DType>*/::addSubTexture(std::string_view name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV) {
	m_subTextures.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
}

//template<class DerivedTexture2DType>
const BaseTexture2D/*<DerivedTexture2DType>*/::SubTexture2D& BaseTexture2D/*<DerivedTexture2DType>*/::getSubTexture(std::string_view name) const {
	auto it = m_subTextures.find(std::string(name));

	if (it != m_subTextures.end())
	{
		return it->second;
	}

	const static SubTexture2D defaultSubTexture;
	return defaultSubTexture;
}

//template<class DerivedTexture2DType>
uint32_t BaseTexture2D/*<DerivedTexture2DType>*/::getWidth() const {
	return m_width;
}

//template<class DerivedTexture2DType>
uint32_t BaseTexture2D/*<DerivedTexture2DType>*/::getHeight() const {
	return m_height;
}