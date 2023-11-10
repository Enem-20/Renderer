#pragma once

#ifndef COLORRESOURCES
#define COLORRESOURCES

#include "ImageProcessing.h"

class SwapChain;
class PhysicalDevice;
class LogicalDevice;
class CommandPool;

class ColorResources : public ImageProcessing{
public:
	ColorResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, SwapChain& swapchain);
	~ColorResources();

private:
};

#endif