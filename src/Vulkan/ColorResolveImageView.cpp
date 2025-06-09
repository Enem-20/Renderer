#include "ColorResolveImageView.h"

ColorResolveImageView::ColorResolveImageView(Device* logicalDevice, Image* image) 
    : ImageView(logicalDevice, image, 1, VK_IMAGE_ASPECT_COLOR_BIT)
{

}