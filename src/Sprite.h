#pragma once

#ifdef OGL
#include <glad/glad.h>
#endif
#include "../../src/ExportPropety.h"

#ifdef OGL
#include "OGL/VertexBuffer.h"
#include "OGL/IndexBuffer.h"
#include "OGL/VertexArray.h"
#elif GLFW_INCLUDE_VULKAN
#include "Vulkan/VertexBuffer.h"
#include "Vulkan/IndexBuffer.h"
#endif

#include "../../internal/ComponentSystem/src/Component.h"
#include "../../internal/Renderer/src/UniformBufferObject.h"

#include <memory>
#include <string>


#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

class Serializer;
class DeserializerSprite;


class Texture2D;
class UniformBuffers;
class RenderPipeline;
class ShaderProgram;
class SwapChain;
class Mesh;

class DLLEXPORT Sprite : public Component
{
	friend Serializer;
	friend DeserializerSprite;
#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN)
public:
	void Translate(glm::vec3 position);
	void Rotate(glm::vec3 rotation);
	void Scale(glm::vec3 scale);
	
	glm::vec2 getSize() const;
	glm::vec3 getRotation() const;
	UniformBufferObject&& getUBO();

	void setPosition(const glm::vec3& position);
	void setSize(const glm::vec2& size);
	void setRotation(const glm::vec3& rotation);

	virtual void render(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline, uint32_t currentFrame);

	void Awake(){}
	void Start() {}
	void Update(uint32_t currentImage);
	void FixedUpdate() {}
	void LastUpdate() {}

	GENERATETYPE(Sprite)
protected:
	std::shared_ptr<Texture2D> m_Texture;
	std::shared_ptr<ShaderProgram> m_shaderProgram;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec2 m_size;
	glm::mat4 model;

	std::string m_subTextureName;
	std::shared_ptr<VertexBuffer> m_vertexCoordsBuffer;
	//VertexBuffer m_textureCoordsBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<UniformBuffers> uniformBuffers;
#endif
#ifdef OGL
public:
	Sprite(std::shared_ptr<Texture2D> Texture,
		std::string initialSubTexture,
		std::shared_ptr<ShaderProgram> shaderProgram,
		const glm::vec2& position = glm::vec2(0.f, 0.f),
		const glm::vec3& rotation = glm::vec3(1.f),
		const glm::vec2& size = glm::vec2(1.f),
		const int RenderMode = GL_DYNAMIC_DRAW);
	virtual ~Sprite();

	Sprite() = delete;
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite&) = delete;
	Sprite(Sprite&& sprite) noexcept;


	virtual void InstanceRender(glm::mat4 model) const;

	int GetRenderMode() const;
protected:
	VertexArray m_vertexArray;
	int RenderMode;
#elif GLFW_INCLUDE_VULKAN
public:
	Sprite(const std::string& name, std::shared_ptr<GameObject> gameObject, 
		std::shared_ptr<Texture2D> Texture,
		std::string initialSubTexture,
		std::shared_ptr<ShaderProgram> shaderProgram,
		std::shared_ptr<Mesh> mesh = nullptr,
		const glm::vec3& position = glm::vec3(0.f, 0.f, 0.0f),
		const glm::vec3& rotation = glm::vec3(1.f),
		const glm::vec2& size = glm::vec2(1.f)/*,
		const int RenderMode = GL_DYNAMIC_DRAW*/);
	virtual ~Sprite();

	Sprite() = delete;
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite&) = delete;
	Sprite(Sprite&& sprite) noexcept;

	//virtual void render(const glm::mat4& model) const;
	//virtual void InstanceRender(glm::mat4 model) const;

	//int GetRenderMode() const;

private:
	std::shared_ptr<Mesh> mesh;
#endif

};
