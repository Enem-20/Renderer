#include "VulkanSprite.h"

#include "Resources/ResourceManager.h"
#include "GameTypes/GameObject.h"
#include "ComponentSystem/Transform.h"

#include "VulkanRenderer.h"
#include "../BaseRenderer.h"

#include "CommandBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "DescriptorPool.h"
#include "VulkanShaderProgram.h"
#include "SwapChain.h"
#include "../BaseTexture2D.h"
#include "Resources/Mesh.h"
#include "../UniformBufferObject.h"

#include <glfw/glfw3.h>

//#include <glm/mat4x4.hpp>
//#include <glm/gtc/matrix_transform.hpp>

VulkanSprite::VulkanSprite(std::string_view name, std::shared_ptr<GameObject> gameObject,
	std::shared_ptr<BaseTexture2D> Texture,
	std::string_view initialSubTexture,
	std::shared_ptr<ShaderProgram> shaderProgram,
	std::shared_ptr<Mesh> mesh = nullptr,
	const glm::vec3& position = glm::vec3(0.f, 0.f, 0.0f),
	const glm::vec3& rotation = glm::vec3(1.f),
	const glm::vec2& size = glm::vec2(1.f))
	: BaseSprite(name, gameObject,
				 Texture, initialSubTexture,
				 shaderProgram, mesh,
				 position, rotation, size)
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

void VulkanSprite::Translate(const glm::vec3& position)
{
	model = glm::mat4(1.f);

	m_position += position;

	model = glm::rotate(model, glm::radians(0.f), m_rotation);
	model = glm::translate(model, m_position);
	model = glm::scale(model, glm::vec3(m_size, 0.f));
}

void VulkanSprite::Rotate(const glm::vec3& rotation)
{

}

void VulkanSprite::Scale(const glm::vec3& scale)
{

}

VulkanSprite::~VulkanSprite()
{}

void VulkanSprite::setPosition(const glm::vec3& position)
{
	m_position = position;
}
void VulkanSprite::setSize(const glm::vec2& size)
{
	m_size = size;

	glm::mat4 buf = glm::mat4(1.f);

	model = glm::rotate(buf, glm::radians(0.f), m_rotation);
	model = glm::translate(buf, m_position);
	model = glm::scale(buf, glm::vec3(m_size, 0.f));
}
void VulkanSprite::setRotation(const glm::vec3& rotation)
{
	m_rotation = rotation;
}

glm::vec2& VulkanSprite::getSize()
{
	return m_size;
}

glm::vec3& VulkanSprite::getRotation()
{
	return m_rotation;
}

glm::vec3& VulkanSprite::getPosition() {
	return m_position;
}

std::shared_ptr<ShaderProgram> VulkanSprite::getShaderProgram() {
	return m_shaderProgram;
}

std::shared_ptr<BaseTexture2D> VulkanSprite::getTexture() {
	return m_Texture;
}

std::shared_ptr<Mesh> VulkanSprite::getMesh() {
	return mesh;
}

std::string& VulkanSprite::getSubTextureName() {
	return m_subTextureName;
}

UniformBufferObject* VulkanSprite::getUBO() {
	return new UniformBufferObject { gameObject.lock()->getComponent<Transform>(gameObject.lock()->name)->GetModel() * model, glm::mat4(1.0f), glm::mat4(1.f) };
}

void VulkanSprite::render()
{
	auto renderer = static_cast<VulkanRenderer*>(&RendererManager::getRenderer());
	auto currentFrame = renderer->currentFrame;
	auto commandBuffer = CommandBuffer();
	commandBuffer.getRaw() = renderer->getCommandBuffers()->getRaw()[currentFrame];
	auto renderPipeline = renderer->getRenderPipeline();

	m_vertexCoordsBuffer->bind(commandBuffer);
	m_IndexBuffer->bind(commandBuffer);

	uniformBuffers->bind(commandBuffer, *renderPipeline, currentFrame);
	m_Texture->bind();

	vkCmdDrawIndexed(commandBuffer.getRaw(), m_IndexBuffer->getIndices().size(), 1, 0, 0, 0);
}

void Sprite::Update(uint32_t currentFrame) {
	uniformBuffers->updateUniformBuffer(currentFrame, getUBO());
}