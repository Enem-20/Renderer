#include "Sprite.h"

#include "Resources/ResourceManager.h"
#include "GameTypes/GameObject.h"
#include "ComponentSystem/Transform.h"

#include "Renderer.h"

#include "Vulkan/VertexBuffer.h"
#include "Vulkan/IndexBuffer.h"
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/DescriptorPool.h"
#include "BaseShaderProgram.h"
#include "Vulkan/SwapChain.h"
#include "Texture2D.h"
#include "Resources/Mesh.h"
#include "UniformBufferObject.h"

#include <glfw/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef OGL
Sprite::Sprite(std::shared_ptr<Texture2D> Texture,
	std::string initialSubTexture,
	std::shared_ptr<ShaderProgram> shaderProgram,
	const glm::vec2& position,
	const glm::vec3& rotation,
	const glm::vec2& size,
	const int RenderMode)
	: m_Texture(std::move(Texture))
	, m_shaderProgram(std::move(shaderProgram))
	, m_position(position)
	, m_rotation(rotation)
	, m_size(size)
	, m_subTextureName(initialSubTexture)
	, RenderMode(RenderMode)
{
	model = glm::mat4(1.f);

	model = glm::rotate(model, glm::radians(0.f), m_rotation);
	model = glm::translate(model, glm::vec3(m_position, 0.f));
	model = glm::scale(model, glm::vec3(m_size, 0.f));

	GLfloat vertexCoords[] =
	{
		//x  y
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f
	};

	auto subTexture = m_Texture->getSubTexture(std::move(initialSubTexture));

	GLfloat textureCoords[] =
	{
		//u  v
		subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
		subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
		subTexture.rightTopUV.x, subTexture.rightTopUV.y,
		subTexture.rightTopUV.x, subTexture.leftBottomUV.y
	};

	GLuint indices[] =
	{
		0,1,2,
		2,3,0
	};


	m_vertexCoordsBuffer.init(vertexCoords, 2 * 4 * sizeof(GL_FLOAT), RenderMode);
	VertexBufferLayout vertexCoordsLayout;
	vertexCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

	m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GL_FLOAT), RenderMode);
	VertexBufferLayout textureCoordsLayout;
	textureCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

	m_IndexBuffer.init(indices, 6);

	m_vertexArray.unbind();
	m_IndexBuffer.unbind();
}

Sprite::Sprite(const Sprite& sprite)
	: RenderMode(sprite.RenderMode)
{
	model = sprite.model;
	m_Texture = sprite.m_Texture;
	m_shaderProgram = sprite.m_shaderProgram;
	m_position = sprite.m_position;
	m_size = sprite.m_size;
	m_rotation = sprite.m_rotation;

	GLfloat vertexCoords[] =
	{
		//x  y
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f
	};

	auto subTexture = m_Texture->getSubTexture(std::move(m_subTextureName));

	GLfloat textureCoords[] =
	{
		//u  v
		subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
		subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
		subTexture.rightTopUV.x, subTexture.rightTopUV.y,
		subTexture.rightTopUV.x, subTexture.leftBottomUV.y
	};

	GLuint indices[] =
	{
		0,1,2,
		2,3,0
	};


	m_vertexCoordsBuffer.init(vertexCoords, 2 * 4 * sizeof(GL_FLOAT), RenderMode);
	VertexBufferLayout vertexCoordsLayout;
	vertexCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

	m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GL_FLOAT), RenderMode);
	VertexBufferLayout textureCoordsLayout;
	textureCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

	m_IndexBuffer.init(indices, 6);

	m_vertexArray.unbind();
	m_IndexBuffer.unbind();
}

Sprite::Sprite(Sprite&& sprite) noexcept
	: m_Texture(std::move(sprite.m_Texture))
	, m_shaderProgram(std::move(sprite.m_shaderProgram))
	, m_position(std::move(sprite.m_position))
	, m_size(std::move(sprite.m_size))
	, m_rotation(std::move(sprite.m_rotation))
	, m_vertexArray(std::move(sprite.m_vertexArray))
	, m_vertexCoordsBuffer(std::move(sprite.m_vertexCoordsBuffer))
	, m_textureCoordsBuffer(std::move(sprite.m_textureCoordsBuffer))
	, m_IndexBuffer(std::move(sprite.m_IndexBuffer))
	, RenderMode(std::move(sprite.RenderMode))
{
	//m_Texture = std::make_shared<RenderEngine::Texture2D>(std::move(*sprite.m_Texture));
	//m_shaderProgram = std::make_shared<RenderEngine::ShaderProgram>(std::move(*sprite.m_shaderProgram));
	//m_position = std::move(sprite.m_position);
	//m_size = std::move(sprite.m_size);
	//m_rotation = std::move(sprite.m_rotation);

	//m_vertexArray = std::move(sprite.m_vertexArray);
	//m_vertexCoordsBuffer = std::move(sprite.m_vertexCoordsBuffer);
	//m_textureCoordsBuffer = std::move(sprite.m_textureCoordsBuffer);
	//m_IndexBuffer = std::move(sprite.m_IndexBuffer);
	//RenderMode = sprite.RenderMode;
}

void Sprite::render(const glm::mat4& model) const
{
	m_shaderProgram->use();

	m_shaderProgram->setMatrix4("modelMat", /*this->*/model);

	glActiveTexture(GL_TEXTURE0);
	m_Texture->bind();

	Renderer::draw(m_vertexArray, m_IndexBuffer, *m_shaderProgram);
}

void Sprite::InstanceRender(glm::mat4 model) const
{
	m_shaderProgram->setMatrix4("modelMat", model);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

int Sprite::GetRenderMode() const
{
	switch (RenderMode)
	{
	case GL_STATIC_DRAW:
		return 0;
		break;
	case GL_DYNAMIC_DRAW:
		return 1;
		break;
	case GL_STREAM_DRAW:
		return 2;
		break;
	default:
		return -1;
	}
}
#endif

Sprite::Sprite(std::string_view name, std::shared_ptr<GameObject> gameObject,
	std::shared_ptr<Texture2D> Texture,
	std::string_view initialSubTexture,
	std::shared_ptr<ShaderProgram> shaderProgram,
	std::shared_ptr<Mesh> mesh,
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec2& size/*,
	const int RenderMode*/)
	: m_Texture(std::move(Texture))
	, m_shaderProgram(std::move(shaderProgram))
	, mesh(mesh)
	, m_position(position)
	, m_rotation(rotation)
	, m_size(size)
	, m_subTextureName(initialSubTexture)
	, m_vertexCoordsBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, Component(name, gameObject)
	//, RenderMode(RenderMode)
{
	auto logicalDevice = ResourceManager::getResource<LogicalDevice>("TestLogicalDevice");
	auto commandPool = ResourceManager::getResource<CommandPool>("TestCommandPool");
	auto swapchain = ResourceManager::getResource<SwapChain>("TestSwapChain");
	auto descriptorPool = ResourceManager::getResource<DescriptorPool>("TestDescriptorPool");
	m_vertexCoordsBuffer = ResourceManager::makeResource<VertexBuffer>(std::string(name) + "VertexBuffer", mesh->vertices, *logicalDevice, *commandPool);
	m_IndexBuffer = ResourceManager::makeResource<IndexBuffer>(std::string(name) + "IndexBuffer", mesh->indices, *logicalDevice, *commandPool);
	uniformBuffers = ResourceManager::makeResource<UniformBuffers>(std::string(name) + "UniformBuffers", *logicalDevice, *swapchain);


	model = glm::mat4(1.f);

	model = glm::rotate(model, glm::radians(0.f), m_rotation);
	model = glm::translate(model, m_position);

	auto subTexture = m_Texture->getSubTexture(initialSubTexture);

#ifdef OGL	
	
	GLfloat vertexCoords[] =
	{
		//x  y
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f
	};
	GLfloat textureCoords[] =
	{
		//u  v
		subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
		subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
		subTexture.rightTopUV.x, subTexture.rightTopUV.y,
		subTexture.rightTopUV.x, subTexture.leftBottomUV.y
	};

	GLuint indices[] =
	{
		0,1,2,
		2,3,0
	};
#endif


	//m_vertexCoordsBuffer.init(vertexCoords, 2 * 4 * sizeof(GL_FLOAT), RenderMode);
	//VertexBufferLayout vertexCoordsLayout;
	//vertexCoordsLayout.addElementLayoutFloat(2, false);
	//m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

	//m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GL_FLOAT), RenderMode);
	//VertexBufferLayout textureCoordsLayout;
	//textureCoordsLayout.addElementLayoutFloat(2, false);
	//m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

	//m_IndexBuffer.init(indices, 6);

	//m_vertexArray.unbind();
	//m_IndexBuffer.unbind();

	uniformBuffers->createDescriptorSets(*descriptorPool);
	m_Texture->createDescriptorSets(*descriptorPool);

	ResourceManager::addResource<Sprite>(this);
}

void Sprite::Translate(const glm::vec3& position)
{
	model = glm::mat4(1.f);

	m_position += position;

	model = glm::rotate(model, glm::radians(0.f), m_rotation);
	model = glm::translate(model, m_position);
	model = glm::scale(model, glm::vec3(m_size, 0.f));
}

void Sprite::Rotate(const glm::vec3& rotation)
{

}

void Sprite::Scale(const glm::vec3& scale)
{

}

Sprite::~Sprite()
{}

void Sprite::setPosition(const glm::vec3& position)
{
	m_position = position;
}
void Sprite::setSize(const glm::vec2& size)
{
	m_size = size;

	glm::mat4 buf = glm::mat4(1.f);

	model = glm::rotate(buf, glm::radians(0.f), m_rotation);
	model = glm::translate(buf, m_position);
	model = glm::scale(buf, glm::vec3(m_size, 0.f));
}
void Sprite::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

glm::vec2& Sprite::getSize()
{
	return m_size;
}

glm::vec3& Sprite::getRotation()
{
	return m_rotation;
}

glm::vec3& Sprite::getPosition(){
	return m_position;
}

std::shared_ptr<ShaderProgram> Sprite::getShaderProgram() {
	return m_shaderProgram;
}

std::shared_ptr<Texture2D> Sprite::getTexture() {
	return m_Texture;
}

std::shared_ptr<Mesh> Sprite::getMesh() {
	return mesh;
}

std::string& Sprite::getSubTextureName() {
	return m_subTextureName;
}

UniformBufferObject* Sprite::getUBO()  {
	return new UniformBufferObject{ gameObject.lock()->getComponent<Transform>(gameObject.lock()->name)->GetModel() * model, glm::mat4(1.0f), glm::mat4(1.f) };
}

void Sprite::render(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline, uint32_t currentFrame)
{
	m_vertexCoordsBuffer->bind(commandBuffer);
	m_IndexBuffer->bind(commandBuffer);

	uniformBuffers->bind(commandBuffer, renderPipeline, currentFrame);
	m_Texture->bind(commandBuffer, renderPipeline, currentFrame);

	vkCmdDrawIndexed(commandBuffer.getRaw(), m_IndexBuffer->getIndices().size(), 1, 0, 0, 0);
}

void Sprite::Update(uint32_t currentImage) {
	uniformBuffers->updateUniformBuffer(currentImage, getUBO());
}