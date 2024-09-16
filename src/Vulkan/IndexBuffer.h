#pragma once

#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"

#include <vector>
#include <memory>

class LogicalDevice;
class CommandPool;
class CommandBuffer;

struct VkBuffer_T;
typedef VkBuffer_T* VkBuffer;

struct VkDeviceMemory_T;
struct VkBuffer_T;
typedef VkDeviceMemory_T* VkDeviceMemory;
typedef VkBuffer_T* VkBuffer;

class DLLEXPORT IndexBuffer : public ResourceBase{
public:
	IndexBuffer(const std::string& name, const std::vector<uint32_t>& indices, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~IndexBuffer();

	void bind(CommandBuffer& commandBuffer);

	VkBuffer& getRaw();
	const std::vector<uint32_t>& getIndices();

	GENERATETYPE(IndexBuffer)
private:
	std::vector<uint32_t> indices{};

	LogicalDevice& logicalDevice;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
};

#endif