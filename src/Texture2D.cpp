#include "Texture2D.h"

#include "../../src/Resources/ResourceManager.h"

#include "Vulkan/SingleTimeBuffer.h"
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/LogicalDevice.h"
#include "Vulkan/WindowSurface.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include <iostream>

Texture2D::SubTexture2D::SubTexture2D(const SubTexture2D& sub)
	: leftBottomUV(sub.leftBottomUV)
	, rightTopUV(sub.rightTopUV)
{}

Texture2D::SubTexture2D::SubTexture2D(SubTexture2D&& sub) noexcept
	: leftBottomUV(sub.leftBottomUV)
	, rightTopUV(sub.rightTopUV)
{}

Texture2D::SubTexture2D::SubTexture2D(const glm::vec2 _leftBottomUV, const glm::vec2 _rightTopUV)
	: leftBottomUV(_leftBottomUV)
	, rightTopUV(_rightTopUV)
{}
Texture2D::SubTexture2D::SubTexture2D()
	: leftBottomUV(0.f)
	, rightTopUV(1.f)
{}

const Texture2D::SubTexture2D& Texture2D::SubTexture2D::operator=(const SubTexture2D& second) { leftBottomUV = second.leftBottomUV; rightTopUV = second.rightTopUV; return *this; }

const glm::vec2& Texture2D::SubTexture2D::getLeftBottomUV() const
{
	return leftBottomUV;
}
const glm::vec2& Texture2D::SubTexture2D::getRightTopUV() const
{
	return rightTopUV;
}

void Texture2D::addSubTexture(const std::string& name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
{
	m_subTextures.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
}
const Texture2D::SubTexture2D& Texture2D::getSubTexture(const std::string& name) const
{
	auto it = m_subTextures.find(name);

	if (it != m_subTextures.end())
	{
		return it->second;
	}

	const static SubTexture2D defaultSubTexture;
	return defaultSubTexture;
}

unsigned int Texture2D::getWidth() const { return m_width; }
unsigned int Texture2D::getHeight() const { return m_height; }

#ifdef GLFW_INCLUDE_VULKAN
Texture2D::Texture2D(const std::string& name, int texWidth, int texHeight, int texChannels, unsigned char* pixels, SwapChain& swapchain, PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool)
	: swapchain(swapchain)
	, physicalDevice(physicalDevice)
	, logicalDevice(logicalDevice)
	, commandPool(commandPool)
	, m_width(texWidth)
	, m_height(texHeight)
	, ResourceBase(name)
{
	//int texWidth, texHeight, texChannels;
	//stbi_uc* pixels = stbi_load("C:/Projects/VulkanTest/build/Debug/res/textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = getImageSize();

	//if (!pixels) {
	//	throw std::runtime_error("failed to load texture image!");
	//}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	logicalDevice.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicalDevice.getRaw(), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(logicalDevice.getRaw(), stagingBufferMemory);

	//stbi_image_free(pixels);

	createImage(texWidth, texHeight, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	transitionImageLayout(textureImage, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
	transitionImageLayout(textureImage, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(logicalDevice.getRaw(), stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), stagingBufferMemory, nullptr);

	createTextureImageView();
	createTextureSampler();

	ResourceManager::addResource<Texture2D>(this);
}

Texture2D::Texture2D(const Texture2D& texture2D) 
	: swapchain(texture2D.swapchain)
	, physicalDevice(texture2D.physicalDevice)
	, logicalDevice(texture2D.logicalDevice)
	, commandPool(texture2D.commandPool)
	, m_width(texture2D.m_width)
	, m_height(texture2D.m_height)
	, m_subTextures(texture2D.m_subTextures)
	, ResourceBase(texture2D.name)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize imageSize = getImageSize();

	logicalDevice.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	transitionImageLayout(textureImage, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	copyImageToBuffer(stagingBuffer, texture2D.textureImage, texture2D.m_width, texture2D.m_height);
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height));
	transitionImageLayout(textureImage, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(logicalDevice.getRaw(), stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), stagingBufferMemory, nullptr);

	createTextureImageView();
	createTextureSampler();

	ResourceManager::addResource<Texture2D>(this);
}

//Texture2D::Texture2D(Texture2D&& texture2D) noexcept
//	: swapchain(std::move(texture2D.swapchain))
//{
//
//}

void Texture2D::createTextureImageView() {
	textureImageView = createImageView(textureImage, /*VK_FORMAT_R8G8B8A8_SRGB*/swapchain.getSwapChainImageFormat());
}

Texture2D::~Texture2D() {
	vkDestroySampler(logicalDevice.getRaw(), textureSampler, nullptr);
	vkDestroyImageView(logicalDevice.getRaw(), textureImageView, nullptr);

	vkDestroyImage(logicalDevice.getRaw(), textureImage, nullptr);
	vkFreeMemory(logicalDevice.getRaw(), textureImageMemory, nullptr);

	ResourceManager::removeResource<Texture2D>(name);
}

void Texture2D::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(logicalDevice.getRaw(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(logicalDevice.getRaw(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = physicalDevice.findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(logicalDevice.getRaw(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(logicalDevice.getRaw(), image, imageMemory, 0);
}

void Texture2D::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	SingleTimeBuffer singleTimeBuffer(logicalDevice, commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;

	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		singleTimeBuffer.getCommandBuffer().getRaw(),
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);
}

void Texture2D::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	SingleTimeBuffer singleTimeBuffer(logicalDevice, commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0,0,0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(
		singleTimeBuffer.getCommandBuffer().getRaw(),
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
						);
}

void Texture2D::copyImageToBuffer(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	SingleTimeBuffer singleTimeBuffer(logicalDevice, commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0,0,0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyImageToBuffer(singleTimeBuffer.getCommandBuffer().getRaw(),
		image,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		buffer,
		1,
		&region);
}

VkImageView Texture2D::createImageView(VkImage image, VkFormat format) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;

	viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(logicalDevice.getRaw(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

void Texture2D::createTextureSampler() {
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
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(logicalDevice.getRaw(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

VkImage& Texture2D::getTextureImage() {
	return textureImage;
}
VkImageView& Texture2D::getTextureImageView() {
	return textureImageView;
}

VkSampler& Texture2D::getTextureSampler() {
	return textureSampler;
}

uint64_t Texture2D::getImageSize() const {
	return m_width * m_height * 4;
}

#endif //!GLFW_INCLUDE_VULKAN

#ifdef OGL
Texture2D::Texture2D(const GLuint width, const GLuint height,
	const unsigned char* data, const unsigned int channels,
	const GLenum filter, const GLenum wrapMode)
	: m_width(width)
	, m_height(height)
{
	switch (channels)
	{
	case 4:
		m_mode = GL_RGBA;
		break;
	case 3:
		m_mode = GL_RGB;
		break;
	case 1:
		m_mode = GL_RED;
		break;
	default:
		m_mode = GL_RGBA;
		break;
	}

	glGenTextures(1, &m_ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D& Texture2D::operator=(Texture2D&& texture2d) noexcept
{
	glDeleteTextures(1, &m_ID);

	m_ID = texture2d.m_ID;
	texture2d.m_ID = 0;
	m_mode = texture2d.m_mode;
	m_width = texture2d.m_width;
	m_height = texture2d.m_height;

	return *this;
}

Texture2D::Texture2D(Texture2D& texture2d)
{
	//glDeleteTextures(1, &m_ID);
	m_ID = texture2d.m_ID;
	texture2d.m_ID = 0;
	m_mode = texture2d.m_mode;
	m_width = texture2d.m_width;
	m_height = texture2d.m_height;
	m_subTextures = texture2d.m_subTextures;
}

Texture2D::Texture2D(Texture2D&& texture2d) noexcept
	: m_ID(std::move(texture2d.m_ID))
	, m_mode(std::move(texture2d.m_mode))
	, m_width(std::move(texture2d.m_width))
	, m_height(std::move(texture2d.m_height))
	, m_subTextures(std::move(texture2d.m_subTextures))
{
	texture2d.m_ID = 0;
}
Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_ID);
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}
#endif //!OGL