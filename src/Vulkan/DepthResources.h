#pragma once

#ifndef DEPTHRESOURCES
#define DEPTHRESOURCES

#include "ImageProcessing.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class PhysicalDevice;
class LogicalDevice;
class CommandPool;
class SwapChain;

class DepthResources : public ImageProcessing{
public:
	DepthResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, SwapChain& swapchain);
	~DepthResources();
private:
	SwapChain& swapchain;
};

#endif // !DEPTHRESOURCES
