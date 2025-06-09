#ifndef C_GPU_QUEUE_H
#define C_GPU_QUEUE_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

class LogicalDevice;
class CommandPool;
class SyncObjects;
class CommandBuffers;

class GPUQueue {
public:
    GPUQueue(LogicalDevice* logicalDevice, uint32_t queueIndex, uint32_t framesInFlight);
    ~GPUQueue();
    VkQueue getRaw() const;
    void waitIdle() const;
    void submit(uint32_t frameIndex) const;
    void present(uint32_t frameIndex, uint32_t imageIndex) const;
private:
    uint32_t queueIndex;
    VkQueue queue;
    LogicalDevice* owner;
};

#endif