#ifndef C_DEPTH_IMAGE_VIEW_H
#define C_DEPTH_IMAGE_VIEW_H

#include "ImageView.h"

class DepthImageView : public ImageView {
public:
    DepthImageView(Device* virtualDevice, Image* image, uint32_t mipLevels);
    
};

#endif