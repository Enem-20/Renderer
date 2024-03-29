#include "VertexBuffer.h"

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "LogicalDevice.h"

#include "Resources/ResourceManager.h"

#include <GLFW/glfw3.h>

VertexBuffer::VertexBuffer(const std::string& name, const std::vector<Vertex>& vertices, LogicalDevice& logicalDevice, CommandPool& commandPool)
	: logicalDevice(logicalDevice)
	, vertices(vertices)
	, ResourceBase(name)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	logicalDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicalDevice.getRaw(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice.getRaw(), stagingBufferMemory);

	logicalDevice.createBuffer(bufferSize
		, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		, vertexBuffer, vertexBufferMemory);

	LogicalDevice::copyBuffer(logicalDevice, commandPool, stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(logicalDevice.getRaw(), stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), stagingBufferMemory, nullptr);

	ResourceManager::addResource<VertexBuffer>(this);
}

VertexBuffer::~VertexBuffer() {
	vkDestroyBuffer(logicalDevice.getRaw(), vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), vertexBufferMemory, nullptr);
}

void VertexBuffer::bind(CommandBuffer& commandBuffer) {
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer.getRaw(), 0, 1, vertexBuffers, offsets);
}

VkBuffer& VertexBuffer::getRaw() {
	return vertexBuffer;
}

const std::vector<Vertex>& VertexBuffer::getVertices() {
	return vertices;
}