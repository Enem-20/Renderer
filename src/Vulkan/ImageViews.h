#ifndef C_IMAGE_VIEWS_H
#define C_IMAGE_VIEWS_H

#include <cstddef>
#include <vector>

#include <vulkan/vulkan.h>

class Device;
class Image;

class ImageViews {
public:
    ImageViews(Device* device, std::vector<Image*> images);

    std::vector<VkImageView>& getRaw();
private:
    std::vector<VkImageView> imageViews;
};

#endif