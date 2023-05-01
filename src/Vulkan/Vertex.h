#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <GLFW\glfw3.h>

#include <array>

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription();

	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};