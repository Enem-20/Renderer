#pragma once

#include "Vertex.h"

#include "Resources/ResourceBase.h"

#include <vector>
#include <memory>

class LogicalDevice;
class CommandPool;
class CommandBuffer;

struct VkBuffer_T;
typedef VkBuffer_T* VkBuffer;
struct VkDeviceMemory_T;
typedef VkDeviceMemory_T* VkDeviceMemory;

class VertexBuffer : public ResourceBase{
public:
	VertexBuffer(const std::string& name, const std::vector<Vertex>& vertices, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~VertexBuffer();

	void bind(CommandBuffer& commandBuffer);

	VkBuffer& getRaw();
	const std::vector<Vertex>& getVertices();

	GENERATETYPE(VertexBuffer)
private:
	LogicalDevice& logicalDevice;
	std::vector<Vertex> vertices{};

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};