#include "IndexBuffer.h"

#include "LogicalDevice.h"

#include "Resources/ResourceManager.h"

#include <GLFW/glfw3.h>

IndexBuffer::IndexBuffer(const std::string& name, const std::vector<uint32_t>& indices, LogicalDevice& logicalDevice, CommandPool& commandPool)
	: logicalDevice(logicalDevice)
	, indices(indices)
	, ResourceBase(name)
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	logicalDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicalDevice.getRaw(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice.getRaw(), stagingBufferMemory);

	logicalDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT/*VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT*/
		, indexBuffer, indexBufferMemory);

	LogicalDevice::copyBuffer(logicalDevice, commandPool, stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(logicalDevice.getRaw(), stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), stagingBufferMemory, nullptr);

	ResourceManager::addResource<IndexBuffer>(this);
}

IndexBuffer::~IndexBuffer() {
	//ResourceManager::removeResource<IndexBuffer>(name);
	vkDestroyBuffer(logicalDevice.getRaw(), indexBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), indexBufferMemory, nullptr);
}

void IndexBuffer::bind(CommandBuffer& commandBuffer) {
	vkCmdBindIndexBuffer(commandBuffer.getRaw(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

VkBuffer& IndexBuffer::getRaw() {
	return indexBuffer;
}

const std::vector<uint32_t>& IndexBuffer::getIndices() {
	return indices;
}