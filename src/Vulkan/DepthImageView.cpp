#include "DepthImageView.h"

#include "Image.h"


DepthImageView::DepthImageView(Device* virtualDevice, Image* image, uint32_t mipLevels) 
    : ImageView(virtualDevice, image, mipLevels, VK_IMAGE_ASPECT_DEPTH_BIT)
{

}