#include "IndexBuffer.h"

#include "LogicalDevice.h"

IndexBuffer::IndexBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool)
	: logicalDevice(logicalDevice)
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
}

IndexBuffer::~IndexBuffer() {
	vkDestroyBuffer(logicalDevice.getRaw(), indexBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), indexBufferMemory, nullptr);
}

VkBuffer& IndexBuffer::getRaw() {
	return indexBuffer;
}

const std::vector<uint16_t>& IndexBuffer::getIndices() {
	return indices;
}