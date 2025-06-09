#pragma once

#ifndef SINGLE_TIME_BUFFER_H
#define SINGLE_TIME_BUFFER_H

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "LogicalDevice.h"

#include "GLFW\glfw3.h"

#include<memory>

class SingleTimeBuffer{
public:
	SingleTimeBuffer() = delete;
	SingleTimeBuffer(LogicalDevice* logicalDevice, CommandPool* commandPool);
	~SingleTimeBuffer();
	void endSingleTimeBuffer();
	CommandBuffer* getCommandBuffer();
private:
	CommandPool* commandPool;
	LogicalDevice* logicalDevice;
	CommandBuffer* commandBuffer;
	bool wasEnded = false;
};

#endif