#pragma once

#ifndef VULKAN_SPRITE
#define VULKAN_SPRITE

#include "../BaseSprite.h"

#include <glm/glm.hpp>

#include <string_view>
#include <memory>

struct UniformBufferObject;

class Serializer;
class DeserializerSprite;


class UniformBuffers;
class RenderPipeline;
class ShaderProgram;
class Mesh;
class CommandBuffer;
class VertexBuffer;
class IndexBuffer;
class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class BaseTexture2D;

class VulkanSprite : public BaseSprite {
public:
	VulkanSprite(std::string_view name, std::shared_ptr<GameObject> gameObject,
		std::shared_ptr<BaseTexture2D> Texture,
		std::string_view initialSubTexture,
		std::shared_ptr<ShaderProgram> shaderProgram,
		std::shared_ptr<Mesh> mesh = nullptr,
		const glm::vec3& position = glm::vec3(0.f, 0.f, 0.0f),
		const glm::vec3& rotation = glm::vec3(1.f),
		const glm::vec2& size = glm::vec2(1.f));
	virtual ~VulkanSprite() {}

	VulkanSprite() = delete;
	VulkanSprite& operator=(const VulkanSprite&) = delete;

	VulkanSprite(const VulkanSprite& sprite);
	VulkanSprite(VulkanSprite&& sprite) noexcept;
public:
	virtual void Translate(const glm::vec3& position) override;
	virtual void Rotate(const glm::vec3& rotation) override;
	virtual void Scale(const glm::vec3& scale) override;

	virtual glm::vec2& getSize() override;
	virtual glm::vec3& getRotation() override;
	virtual glm::vec3& getPosition() override;
	virtual UniformBufferObject* getUBO() override;
	virtual std::shared_ptr<ShaderProgram> getShaderProgram() override;
	virtual std::shared_ptr<BaseTexture2D> getTexture() override;
	virtual std::shared_ptr<Mesh> getMesh() override;
	virtual std::string& getSubTextureName() override;

	virtual void setPosition(const glm::vec3& position) override;
	virtual void setSize(const glm::vec2& size) override;
	virtual void setRotation(const glm::vec3& rotation) override;

	virtual void render() override;

	void Awake() override {}
	void Start() override {}
	void Update(uint32_t currentFrame) override;
	void FixedUpdate() override {}
	void LastUpdate() override {}

	GENERATETYPE(VulkanSprite)
};

#endif