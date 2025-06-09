#ifndef C_COLOR_RESOLVE_IMAGE_VIEW_H
#define C_COLOR_RESOLVE_IMAGE_VIEW_H

#include "ImageView.h"

class ColorResolveImageView : public ImageView {
public:
    ColorResolveImageView(Device* logicalDevice, Image* image);
private:
};

#endif