#pragma once
#include "../../src/ExportPropety.h"


#ifdef OGL
#include "OGL/VertexBuffer.h"
#include "OGL/IndexBuffer.h"
#include "OGL/VertexArray.h"
#elif GLFW_INCLUDE_VULKAN
#include "Vulkan/VertexBuffer.h"
#include "Vulkan/IndexBuffer.h"
#endif

#include <memory>
#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

class Serializer;
class DeserializerSprite;


class Texture2D;
class ShaderProgram;

class DLLEXPORT Sprite
{
	friend Serializer;
	friend DeserializerSprite;
#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN)
public:
	void Translate(glm::vec3 position);
	void Rotate(glm::vec3 rotation);
	void Scale(glm::vec3 scale);

	void setPosition(const glm::vec2& position);
	glm::vec2 getSize() const;
	glm::vec3 getRotation() const;
	void setSize(const glm::vec2& size);
	void setRotation(const glm::vec3& rotation);

	std::string name;
protected:
	std::shared_ptr<Texture2D> m_Texture;
	std::shared_ptr<ShaderProgram> m_shaderProgram;
	glm::vec2 m_position;
	glm::vec3 m_rotation;
	glm::vec2 m_size;
	glm::mat4 model;

	std::string m_subTextureName;
	VertexBuffer m_vertexCoordsBuffer;
	VertexBuffer m_textureCoordsBuffer;
	IndexBuffer m_IndexBuffer;
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

	virtual void render(const glm::mat4& model) const;
	virtual void InstanceRender(glm::mat4 model) const;

	int GetRenderMode() const;
protected:
	VertexArray m_vertexArray;
	int RenderMode;
#elif GLFW_INCLUDE_VULKAN
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

	virtual void render(const glm::mat4& model) const;
	virtual void InstanceRender(glm::mat4 model) const;

	int GetRenderMode() const;
#endif

};
