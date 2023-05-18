#pragma once

#include "Vertex.h"

#include "../../src/Resources/ResourceBase.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class LogicalDevice;
class CommandPool;
class CommandBuffer;


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
	/*const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};*/
	std::vector<Vertex> vertices{};

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};