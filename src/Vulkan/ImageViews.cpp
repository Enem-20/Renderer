#include "ImageViews.h"


ImageViews::ImageViews(Device* device, std::vector<Image*> images) {

}

std::vector<VkImageView>& ImageViews::getRaw() {
    return imageViews;
}