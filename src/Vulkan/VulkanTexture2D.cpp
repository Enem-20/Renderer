#include "VulkanTexture2D.h"

#include "Resources/ResourceManager.h"

#include "RenderPipeline.h"
#include "DescriptorPool.h"
#include "SingleTimeBuffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "WindowSurface.h"
#include "GeneralVulkanStorage.h"
#include "../RendererManager.h"
#include "VulkanRenderer.h"

#include <GLFW/glfw3.h>

#include <cstddef>
#include <iostream>
#include <tuple>

VkDescriptorSetLayout VulkanTexture2D::descriptorSetLayout;

VulkanTexture2D::VulkanTexture2D(std::string_view name, const std::string& relativePath, uint32_t texWidth, uint32_t texHeight, unsigned char* pixels, uint32_t texChannels, const std::unordered_map<size_t, std::string_view>& dependencies_names)
	: BaseTexture2D(name, relativePath, texWidth, texHeight, texChannels)
	, physicalDevice(*ResourceManager::getResource<PhysicalDevice>(dependencies_names.at(PhysicalDevice::type_hash)))
	, logicalDevice(*ResourceManager::getResource<LogicalDevice>(dependencies_names.at(LogicalDevice::type_hash)))
	, swapchain(*ResourceManager::getResource<SwapChain>(dependencies_names.at(SwapChain::type_hash)))
	, ImageProcessing(physicalDevice, logicalDevice, *ResourceManager::getResource<CommandPool>(dependencies_names.at(CommandPool::type_hash)))
{
	VkDeviceSize imageSize = getImageSize();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	logicalDevice.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicalDevice.getRaw(), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(logicalDevice.getRaw(), stagingBufferMemory);

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	createImage(texWidth, texHeight, /*VK_FORMAT_R8G8B8A8_SRGB*/mipLevels, static_cast<size_t>(VK_SAMPLE_COUNT_1_BIT), swapchain.getSwapChainImageFormat(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory);

	transitionImageLayout(image, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
	copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

	generateMipmaps(VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, physicalDevice, commandPool);

	vkDestroyBuffer(logicalDevice.getRaw(), stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), stagingBufferMemory, nullptr);

	createTextureImageView();
	createTextureSampler();

	ResourceManager::addResource<BaseTexture2D>(this);
}

void VulkanTexture2D::createDescriptorSetLayout(LogicalDevice& logicalDevice) {
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &samplerLayoutBinding;

	if (vkCreateDescriptorSetLayout(logicalDevice.getRaw(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void VulkanTexture2D::destroyDescriptorSetLayout(LogicalDevice& logicalDevice) {
	vkDestroyDescriptorSetLayout(logicalDevice.getRaw(), descriptorSetLayout, nullptr);
}

VkDescriptorSetLayout& VulkanTexture2D::getDescriptorSetLayout() {
	return descriptorSetLayout;
}

VulkanTexture2D::VulkanTexture2D(const VulkanTexture2D& texture2D)
	: BaseTexture2D(texture2D)
	, ImageProcessing(texture2D.physicalDevice, texture2D.logicalDevice, texture2D.commandPool)
	, physicalDevice(texture2D.physicalDevice)
	, logicalDevice(texture2D.logicalDevice)
	, swapchain(texture2D.swapchain)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize imageSize = getImageSize();

	logicalDevice.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(m_width, m_height)))) + 1;
	transitionImageLayout(image, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, mipLevels);
	copyImageToBuffer(stagingBuffer, texture2D.image, texture2D.m_width, texture2D.m_height);
	copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height));

	generateMipmaps(VK_FORMAT_R8G8B8A8_SRGB, m_width, m_height, physicalDevice, commandPool);

	vkDestroyBuffer(logicalDevice.getRaw(), stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), stagingBufferMemory, nullptr);

	createTextureImageView();
	createTextureSampler();

	ResourceManager::addResource<BaseTexture2D>(this);
}

VulkanTexture2D::~VulkanTexture2D() {
	vkDestroySampler(logicalDevice.getRaw(), textureSampler, nullptr);
}

void VulkanTexture2D::bind() {
	auto renderer = static_cast<VulkanRenderer*>(&RendererManager::getRenderer());
	auto currentFrame = renderer->currentFrame;
	auto commandBuffer = renderer->getCommandBuffers()->getRaw()[currentFrame];
	auto renderPipeline = renderer->getRenderPipeline();
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderPipeline->getPipelineLayout(), 1, 1,
		&descriptorSets[currentFrame], 0, nullptr);
	renderer = nullptr;
}

VkSampler& VulkanTexture2D::getTextureSampler() {
	return textureSampler;
}

uint64_t VulkanTexture2D::getImageSize() const {
	return m_width * m_height * channels;
}

void VulkanTexture2D::generateMipmaps(VkFormat imageFormat, int32_t texWidth, int32_t texHeight, PhysicalDevice& physicalDevice, CommandPool& commandPool) {
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice.getRaw(), imageFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	SingleTimeBuffer singleTimeBuffer(logicalDevice, commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;



	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(singleTimeBuffer.getCommandBuffer().getRaw(),
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(singleTimeBuffer.getCommandBuffer().getRaw(),
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(singleTimeBuffer.getCommandBuffer().getRaw(),
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);
		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(singleTimeBuffer.getCommandBuffer().getRaw(),
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}

void VulkanTexture2D::createTextureImageView() {
	imageView = createImageView(image, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
}

void VulkanTexture2D::createTextureSampler() {
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physicalDevice.getRaw(), &properties);

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float>(mipLevels);

	if (vkCreateSampler(logicalDevice.getRaw(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

void VulkanTexture2D::createDescriptorSets(DescriptorPool& descriptorPool) {
	std::vector<VkDescriptorSetLayout> layouts(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool.getRaw();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(logicalDevice.getRaw(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT; ++i) {
		VkDescriptorImageInfo imageInfo;

		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = imageView;
		imageInfo.sampler = textureSampler;

		VkWriteDescriptorSet descriptorWrite;

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;//imageInfos.size();
		descriptorWrite.pImageInfo = &imageInfo;
		descriptorWrite.pBufferInfo = nullptr;
		descriptorWrite.pNext = nullptr;

		vkUpdateDescriptorSets(logicalDevice.getRaw(), 1, &descriptorWrite, 0, nullptr);
	}
}