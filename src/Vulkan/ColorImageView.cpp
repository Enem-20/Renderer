#include "ColorImageView.h"
#include "ImageView.h"

ColorImageView::ColorImageView(Device* virtualDevice, Image* image, uint32_t mipLevels) 
    : ImageView(virtualDevice, image, mipLevels, VK_IMAGE_ASPECT_COLOR_BIT)
{

}