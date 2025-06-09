#ifndef C_IMAGE_H
#define C_IMAGE_H

#include <vulkan/vulkan.h>

class Image {
public:
    Image() = delete;
    Image(const Image&) = delete;
    Image(Image&&) = delete;
    Image& operator=(const Image&) = delete;
    Image& operator=(Image&&) = delete;

    Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue,
          uint32_t width, uint32_t height, VkFormat format, uint32_t mipLevels);
    ~Image();

    VkImage getRaw() const;
    VkFormat getRawFormat() const;
private:
    VkImage image;
    VkDeviceMemory imageMemory;
    VkImageView imageView;
    VkFormat format;
    uint32_t width, height, mipLevels;
};

#endif