#ifndef S_FRAME_H
#define S_FRAME_H

#include <cstdint>

#include "vulkan/vulkan.h"

#include "Sync.h"

struct Frame : public Sync {
    Frame(uint32_t frameFlightIndex, VkFence inFlightFence, VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, VkCommandBuffer commandBuffer, uint32_t frameIndex)
        : Sync(frameFlightIndex, inFlightFence, imageAvailableSemaphore, renderFinishedSemaphore)
        , commandBuffer(commandBuffer)
        , frameIndex(frameIndex)
        {}
    Frame(uint32_t frameFlightIndex)
        : Sync(frameFlightIndex)
    {}
    VkCommandBuffer commandBuffer;
    uint32_t frameIndex = 0;
};

#endif