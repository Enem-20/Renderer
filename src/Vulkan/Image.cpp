#include "Image.h"

Image::Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue,
    uint32_t width, uint32_t height, VkFormat format, uint32_t mipLevels) {

    }
Image::~Image() {

}

VkImage Image::getRaw() const {
    return image;
}

VkFormat Image::getRawFormat() const {
    return format;
}