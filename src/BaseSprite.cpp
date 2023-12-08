#include "BaseSprite.h"

#include "Resources/ResourceManager.h"
#include "GameTypes/GameObject.h"
#include "ComponentSystem/Transform.h"

#include "Vulkan/VulkanRenderer.h"
#include "BaseRenderer.h"

#include "Vulkan/VertexBuffer.h"
#include "Vulkan/IndexBuffer.h"
#include "Vulkan/UniformBuffer.h"
#include "Vulkan/DescriptorPool.h"
#include "ShaderProgram.h"
#include "Vulkan/SwapChain.h"
#include "BaseTexture2D.h"
#include "Resources/Mesh.h"
#include "UniformBufferObject.h"

#include <glfw/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

BaseSprite::BaseSprite(std::string_view name, std::shared_ptr<GameObject> gameObject,
	std::shared_ptr<BaseTexture2D> Texture,
	std::string_view initialSubTexture,
	std::shared_ptr<BaseShaderProgram> shaderProgram,
	std::shared_ptr<Mesh> mesh = nullptr,
	const glm::vec3& position = glm::vec3(0.f, 0.f, 0.0f),
	const glm::vec3& rotation = glm::vec3(1.f),
	const glm::vec2& size = glm::vec2(1.f))
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
{
	auto logicalDevice = ResourceManager::getResource<LogicalDevice>("TestLogicalDevice");
	auto commandPool = ResourceManager::getResource<CommandPool>("TestCommandPool");
	auto swapchain = ResourceManager::getResource<SwapChain>("TestSwapChain");
	auto descriptorPool = ResourceManager::getResource<DescriptorPool>("TestDescriptorPool");
	m_vertexCoordsBuffer = ResourceManager::makeResource<VertexBuffer>(this->name + "VertexBuffer", mesh->vertices, *logicalDevice, *commandPool);
	m_IndexBuffer = ResourceManager::makeResource<IndexBuffer>(this->name + "IndexBuffer", mesh->indices, *logicalDevice, *commandPool);
	uniformBuffers = ResourceManager::makeResource<UniformBuffers>(this->name + "UniformBuffers", *logicalDevice, *swapchain);


	model = glm::mat4(1.f);

	model = glm::rotate(model, glm::radians(0.f), m_rotation);
	model = glm::translate(model, m_position);

	auto subTexture = m_Texture->getSubTexture(std::move(initialSubTexture));

	uniformBuffers->createDescriptorSets(*descriptorPool);
	std::static_pointer_cast<VulkanTexture2D>(m_Texture)->createDescriptorSets(*descriptorPool);

	ResourceManager::addResource<BaseSprite>(this);
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

glm::vec3& Sprite::getPosition() {
	return m_position;
}

std::shared_ptr<ShaderProgram> Sprite::getShaderProgram() {
	return m_shaderProgram;
}

std::shared_ptr<BaseTexture2D> Sprite::getTexture() {
	return m_Texture;
}

std::shared_ptr<Mesh> Sprite::getMesh() {
	return mesh;
}

std::string& Sprite::getSubTextureName() {
	return m_subTextureName;
}

UniformBufferObject* Sprite::getUBO() {
	return new UniformBufferObject{ gameObject.lock()->getComponent<Transform>(gameObject.lock()->name)->GetModel() * model, glm::mat4(1.0f), glm::mat4(1.f) };
}

void Sprite::render(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline, uint32_t currentFrame)
{
	m_vertexCoordsBuffer->bind(commandBuffer);
	m_IndexBuffer->bind(commandBuffer);

	uniformBuffers->bind(commandBuffer, renderPipeline, currentFrame);
	m_Texture->bind();

	vkCmdDrawIndexed(commandBuffer.getRaw(), m_IndexBuffer->getIndices().size(), 1, 0, 0, 0);
}

void Sprite::Update(uint32_t currentFrame) {
	uniformBuffers->updateUniformBuffer(currentFrame, getUBO());
}