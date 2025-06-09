#pragma once

#ifndef BASE_SPRITE
#define BASE_SPRITE

#include "ComponentSystem/Component.h"

#include <glm/glm.hpp>

struct UniformBufferObject;

class Serializer;
class DeserializerSprite;


class UniformBuffers;
class RenderPipeline;
class BaseShaderProgram;
class Mesh;
class CommandBuffer;
class VertexBuffer;
class IndexBuffer;
class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class BaseTexture2D;

class BaseSprite : public Component{
protected:
	friend class Serializer;
	friend DeserializerSprite;
public:
	BaseSprite(std::string_view name, std::shared_ptr<GameObject> gameObject,
		std::shared_ptr<BaseTexture2D> Texture,
		std::string_view initialSubTexture,
		std::shared_ptr<BaseShaderProgram> shaderProgram,
		std::shared_ptr<Mesh> mesh = nullptr,
		const glm::vec3& position = glm::vec3(0.f, 0.f, 0.0f),
		const glm::vec3& rotation = glm::vec3(1.f),
		const glm::vec2& size = glm::vec2(1.f));
	virtual ~BaseSprite() {}

	BaseSprite() = delete;
	BaseSprite& operator=(const BaseSprite&) = delete;

	BaseSprite(const BaseSprite& sprite);
	BaseSprite(BaseSprite&& sprite) noexcept;
public:
	virtual void Translate(const glm::vec3& position) = 0;
	virtual void Rotate(const glm::vec3& rotation) = 0;
	virtual void Scale(const glm::vec3& scale) = 0;

	virtual glm::vec2& getSize() = 0;
	virtual glm::vec3& getRotation() = 0;
	virtual glm::vec3& getPosition() = 0;
	virtual UniformBufferObject* getUBO() = 0;
	virtual std::shared_ptr<BaseShaderProgram> getShaderProgram() = 0;
	virtual std::shared_ptr<BaseTexture2D> getTexture() = 0;
	virtual std::shared_ptr<Mesh> getMesh() = 0;
	virtual std::string& getSubTextureName() = 0;

	virtual void setPosition(const glm::vec3& position) = 0;
	virtual void setSize(const glm::vec2& size) = 0;
	virtual void setRotation(const glm::vec3& rotation) = 0;

	virtual void render() = 0;

	void Awake() override {}
	void Start() override {}
	void Update(uint32_t currentFrame) = 0;
	void FixedUpdate() override {}
	void LastUpdate() override {}

	GENERATETYPE(BaseSprite)
protected:
	std::shared_ptr<BaseTexture2D> m_Texture;
	std::shared_ptr<BaseShaderProgram> m_shaderProgram;
	std::shared_ptr<Mesh> mesh;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec2 m_size;
	glm::mat4 model;

	std::string m_subTextureName;
	std::shared_ptr<VertexBuffer> m_vertexCoordsBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<UniformBuffers> uniformBuffers;
};

#endif // !BASE_SPRITE
