#pragma once

#ifndef COLORRESOURCES
#define COLORRESOURCES

#include "ImageProcessing.h"

#include <GLFW/glfw3.h>

class SwapChain;

class ColorResources : public ImageProcessing{
public:
	ColorResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, SwapChain& swapchain);
	~ColorResources();

private:
};

#endif