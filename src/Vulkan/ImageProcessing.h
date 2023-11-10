#pragma once

#ifndef IMAGEPROCESSING
#define IMAGEPROCESSING

#include "ImageView.h"

#include <memory>

class PhysicalDevice;
class LogicalDevice;
class CommandPool;

typedef uint32_t VkFlags;
typedef VkFlags VkImageUsageFlags;
typedef VkFlags VkMemoryPropertyFlags;

struct VkImage_T;
struct VkImageView_T;
struct VkDeviceMemory_T;
struct VkBuffer_T;
typedef VkImage_T* VkImage;
typedef VkImageView_T* VkImageView;
typedef VkDeviceMemory_T* VkDeviceMemory;
typedef VkBuffer_T* VkBuffer;

enum VkFormat;
enum VkImageTiling;
enum VkImageLayout;
enum VkSampleCountFlagBits;

class ImageProcessing : public ImageView{
public:
	ImageProcessing(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool);
	~ImageProcessing();

	VkImage& getImage();
	VkImageView& getImageView();

protected:
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, std::shared_ptr<VkSampleCountFlagBits> numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
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