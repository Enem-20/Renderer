#pragma once

#ifndef IMAGEPROCESSING
#define IMAGEPROCESSING

#include "ImageView.h"

#include <GLFW/glfw3.h>

class PhysicalDevice;
class LogicalDevice;
class CommandPool;

class ImageProcessing : public ImageView{
public:
	ImageProcessing(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~ImageProcessing();

	VkImage& getImage();
	VkImageView& getImageView();

protected:
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	//VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void copyImageToBuffer(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	bool hasStencilComponent(VkFormat format);

	PhysicalDevice& physicalDevice;
	LogicalDevice& logicalDevice;
	CommandPool& commandPool;

	VkImage image;
	VkDeviceMemory imageMemory;
	VkImageView imageView;
};

#endif