#pragma once

#ifndef DEPTHRESOURCES
#define DEPTHRESOURCES

#include "ImageProcessing.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glfw/glfw3.h"

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
