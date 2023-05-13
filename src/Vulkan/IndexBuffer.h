#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include "GLFW/glfw3.h"

#include <vector>
#include <memory>

class LogicalDevice;
class CommandPool;

class DLLEXPORT IndexBuffer : public ResourceBase{
public:
	IndexBuffer(const std::string& name, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~IndexBuffer();

	VkBuffer& getRaw();
	const std::vector<uint16_t>& getIndices();

	inline static const std::string type = GETTYPE(IndexBuffer);
private:
	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
	};

	LogicalDevice& logicalDevice;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
};