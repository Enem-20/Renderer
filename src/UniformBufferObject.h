#pragma once

#ifndef UNIFORMBUFFEROBJECT
#define UNIFORMBUFFEROBJECT

#include <glm/matrix.hpp>

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};
#endif