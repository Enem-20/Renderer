#include "GPUQueue.h"

#include "LogicalDevice.h"
#include "CommandPool.h"
#include "SwapChain.h"
#include "SyncObjects.h"
#include "CommandBuffer.h"
#include "Device.h"
#include "Frame.h"
#include "WindowSurface.h"

GPUQueue::GPUQueue(LogicalDevice* logicalDevice, uint32_t queueIndex, uint32_t framesInFlight)
    : queueIndex(queueIndex), queue(queue), owner(logicalDevice)
{
	vkGetDeviceQueue(logicalDevice->getRaw(), queueIndex, 0, &queue);
}

VkQueue GPUQueue::getRaw() const {
    return queue;
}

void GPUQueue::waitIdle() const {
    vkQueueWaitIdle(queue);
}

void GPUQueue::submit(uint32_t frameIndex) const {
    VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	Frame* frame = owner->getOwner()->instance->getFramesInFlight()[frameIndex];
	VkSemaphore waitSemaphores[] = { frame->imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &(frame->commandBuffer);

	if (vkQueueSubmit(queue, 1, &submitInfo, frame->inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}
}

void GPUQueue::present(uint32_t frameIndex, uint32_t imageIndex) const {
	Frame* frame = owner->getOwner()->instance->getFramesInFlight()[frameIndex];

	VkSemaphore signalSemaphores[] = { frame->renderFinishedSemaphore };
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	SwapChain* swapChain = owner->getOwner()->instance->getWindowSurface("main")->getSwapChain("main");
	VkSwapchainKHR swapChains[] = { swapChain->getRaw() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult result = vkQueuePresentKHR(queue, &presentInfo);
	WindowSurface* windowSurface = owner->getOwner()->instance->getWindowSurface("main");
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowSurface->getFramebufferResized()) {
		windowSurface->setFramebufferResized(false);
		swapChain->recreateSwapChain(renderPass, owner->getCommandPool());
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}