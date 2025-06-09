#ifndef S_SYNC_H
#define S_SYNC_H

#include <cstdint>
#include <vulkan/vulkan_core.h>

#include "FrameFlightResource.h"

struct Sync : public FrameFlightResource {
    Sync(uint32_t frameFlightIndex, VkFence inFlightFence, VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore)
        : FrameFlightResource(frameFlightIndex)
        , inFlightFence(inFlightFence)
        , imageAvailableSemaphore(imageAvailableSemaphore)
        , renderFinishedSemaphore(renderFinishedSemaphore)
        {
        }
    Sync(uint32_t frameFlightIndex)
        : FrameFlightResource(frameFlightIndex)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.flags = 0;
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image available semaphore!");
        }

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    VkFence inFlightFence;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
};

#endif