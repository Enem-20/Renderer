#pragma once

#include "DescriptorSetBase.h"
#include "../../src/Resources/ResourceBase.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class LogicalDevice;
class SwapChain;
class DescriptorPool;
class CommandBuffer;
class RenderPipeline;
class UniformBufferObject;

struct VkDeviceMemory_T;
struct VkBuffer_T;
struct VkDescriptorSetLayout_T;

typedef VkDeviceMemory_T* VkDeviceMemory;
typedef VkBuffer_T* VkBuffer;
typedef VkDescriptorSetLayout_T* VkDescriptorSetLayout;

class UniformBuffers : public ResourceBase, public DescriptorSetBase{
public:
	UniformBuffers(const std::string& name, LogicalDevice& logicalDevice, SwapChain& swapchain);
	~UniformBuffers();

	void bind(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline, uint32_t currentFrame);

	std::vector<VkBuffer>& getRaw();

	static VkDescriptorSetLayout& getDescriptorSetLayout();

	void updateUniformBuffer(uint32_t currentImage, UniformBufferObject* ubo);

	void createDescriptorSets(DescriptorPool& descriptorPool) override;

	GENERATETYPE(UniformBuffers)
	static void createDescriptorSetLayout(LogicalDevice& logicalDevice);
	static void destroyDescriptorSetLayout(LogicalDevice& logicalDevice);
private:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	LogicalDevice& logicalDevice;
	SwapChain& swapchain;

	static VkDescriptorSetLayout descriptorSetLayout;
};