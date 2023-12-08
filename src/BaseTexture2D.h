#pragma once

#ifndef BASE_TEXTURE_2D
#define BASE_TEXTURE_2D

#include "Resources/ResourceBase.h"
#include "DependencyResolvers/DependencyList.h"

#include<glm/glm.hpp>

#include<tuple>
#include<string>
#include<vector>
#include<unordered_map>
#include <functional>

class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class CommandBuffer;
class RenderPipeline;

//template<class DerivedTexture2DType>
class BaseTexture2D : public ResourceBase/*<DerivedTexture2DType>*/{
	friend class Serializer;
	friend class DeserializerTexture2D;
public:
	struct DLLEXPORT SubTexture2D
	{
		std::shared_ptr<glm::vec2> leftBottomUV;
		std::shared_ptr<glm::vec2> rightTopUV;

		SubTexture2D(const SubTexture2D& sub);

		SubTexture2D(SubTexture2D&& sub) noexcept;

		SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV);
		SubTexture2D();

		const glm::vec2& getLeftBottomUV() const;
		const glm::vec2& getRightTopUV() const;

		const SubTexture2D& operator=(const SubTexture2D& second);
	};

public:
	BaseTexture2D(std::string_view name, const std::string& relativePath, const uint32_t& texWidth, const uint32_t& texHeight, const uint32_t& channels);
	BaseTexture2D(const BaseTexture2D& texture2D);
	virtual ~BaseTexture2D(){}

	BaseTexture2D() = delete;
	BaseTexture2D& operator=(const BaseTexture2D&) = delete;

	virtual void bind() = 0;

	void addSubTexture(std::string_view name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
	const SubTexture2D& getSubTexture(std::string_view name) const;
	uint32_t getWidth() const;
	uint32_t getHeight() const;

	std::string m_path;
	GENERATETYPE(BaseTexture2D)
protected:
	uint32_t m_width;
	uint32_t m_height;

	std::unordered_map<std::string, SubTexture2D> m_subTextures;
	std::unordered_map<size_t, std::string_view> dependencies;
	uint32_t channels = 4;
};

#endif