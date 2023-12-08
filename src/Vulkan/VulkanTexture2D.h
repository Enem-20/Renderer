#pragma once

#ifndef VULKAN_TEXTURE2D
#define VULKAN_TEXTURE2D

#include "../BaseTexture2D.h"
#include "DescriptorSetBase.h"
#include "ImageProcessing.h"
#include "Tools/VariadicComparer.h"
#include "DependencyResolvers/DependencyList.h"
#include "Tools/CompileTimeHash.h"

#include <glm/glm.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>
#include <span>

class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class SwapChain;
class DescriptorPool;
class CommandBuffer;
class RenderPipeline;
class BaseTexture2D;
struct VkDescriptorSetLayout_T;
typedef VkDescriptorSetLayout_T* VkDescriptorSetLayout;
struct VkSampler_T;
typedef VkSampler_T* VkSampler;
enum VkFormat;

class VulkanTexture2D : public BaseTexture2D, public ImageProcessing, public DescriptorSetBase{
public:
	VulkanTexture2D(std::string_view name, const std::string& relativePath, uint32_t texWidth, uint32_t texHeight, unsigned char* pixels, uint32_t texChannels, const std::unordered_map<size_t, std::string_view>& dependencies_names);

	VulkanTexture2D(const VulkanTexture2D& texture2D);
	~VulkanTexture2D();

	VkSampler& getTextureSampler();
	uint64_t getImageSize() const;

	static VkDescriptorSetLayout& getDescriptorSetLayout();

	void bind() override;

	static void createDescriptorSetLayout(LogicalDevice& logicalDevice);
	static void destroyDescriptorSetLayout(LogicalDevice& logicalDevice);
	void createDescriptorSets(DescriptorPool& descriptorPool);

	GENERATETYPE(VulkanTexture2D)
private:
	void generateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight, PhysicalDevice& physicalDevice, CommandPool& commandPool);
	void createTextureImageView();
	void createTextureSampler();

	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;
	SwapChain& swapchain;

	uint32_t mipLevels;
	VkSampler textureSampler;

	static VkDescriptorSetLayout descriptorSetLayout;
};

#endif