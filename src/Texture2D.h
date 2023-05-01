#pragma once

#include <glfw/glfw3.h>

#include <memory>

class PhysicalDevice;
class LogicalDevice;
class CommandPool;

class Texture2D {
public:
	Texture2D(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~Texture2D();

	VkImage& getTextureImage();
	VkImageView& getTextureImageView();
	VkSampler& getTextureSampler();
private:
	void createTextureImageView();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VkImageView createImageView(VkImage image, VkFormat format);
	void createTextureSampler();

	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;
	CommandPool& commandPool;

	VkImageView textureImageView;
	VkSampler textureSampler;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
};