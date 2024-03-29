#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "LogicalDevice.h"

#include<memory>

class SingleTimeBuffer{
public:
	SingleTimeBuffer() = delete;
	SingleTimeBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool);
	~SingleTimeBuffer();
	void endSingleTimeBuffer();
	CommandBuffer& getCommandBuffer();
private:
	CommandPool& commandPool;
	LogicalDevice& logicalDevice;
	CommandBuffer commandBuffer;
	bool wasEnded = false;
};