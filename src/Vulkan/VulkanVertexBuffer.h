#pragma once

#ifndef VULKAN_VERTEX_BUFFER_H
#define VULKAN_VERTEX_BUFFER_H

#include "Vertex.h"

#include "../BaseVertexBuffer.h"

#include <vector>
#include <memory>

class LogicalDevice;
class CommandPool;
class CommandBuffer;

struct VkBuffer_T;
typedef VkBuffer_T* VkBuffer;
struct VkDeviceMemory_T;
typedef VkDeviceMemory_T* VkDeviceMemory;

class VulkanVertexBuffer : public BaseVertexBuffer {
public:
	VulkanVertexBuffer(const std::string& name, const std::vector<Vertex>& vertices, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~VulkanVertexBuffer();

	void bind(CommandBuffer* commandBuffer);

	VkBuffer& getRaw();
	const std::vector<Vertex>& getVertices();

	GENERATETYPE(VulkanVertexBuffer)
private:
	LogicalDevice& logicalDevice;
	std::vector<Vertex> vertices{};

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};

#endif