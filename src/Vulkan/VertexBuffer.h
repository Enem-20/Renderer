#pragma once

#include "Vertex.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class LogicalDevice;
class CommandPool;


class VertexBuffer {
public:
	VertexBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool);
	~VertexBuffer();

	VkBuffer& getRaw();
	const std::vector<Vertex>& getVertices();
private:
	LogicalDevice& logicalDevice;
	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};