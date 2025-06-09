#ifndef C_FRAME_BUFFER_H
#define C_FRAME_BUFFER_H

#include "SwapChain.h"
#include <cstddef>
#include <vector>

#include <vulkan/vulkan.h>

class RenderPass;
class ImageViews;

class FrameBuffers
{
public:
    FrameBuffers(size_t count, RenderPass* renderPass, ImageViews* attachments);
    ~FrameBuffers();

    std::vector<VkFramebuffer>& getFramebuffers();
private:
    std::vector<VkFramebuffer> framebuffers; 
};

#endif