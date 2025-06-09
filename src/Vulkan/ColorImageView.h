#ifndef C_COLOR_IMAGE_VIEW_H
#define C_COLOR_IMAGE_VIEW_H

#include "ImageView.h"

class ColorImageView : public ImageView {
public:
    ColorImageView(Device* virtualDevice, Image* image, uint32_t mipLevels);

};

#endif