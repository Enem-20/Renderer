#include "SingleTimeBuffer.h"

#include <iostream>

SingleTimeBuffer::SingleTimeBuffer(LogicalDevice* logicalDevice, CommandPool* commandPool) 
	: commandPool(commandPool) 
	, logicalDevice(logicalDevice)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool->getRaw();
	allocInfo.commandBufferCount = 1;

	commandBuffer = new CommandBuffer();
	vkAllocateCommandBuffers(logicalDevice->getRaw(), &allocInfo, &commandBuffer->getRaw());

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer->getRaw(), &beginInfo);
}

SingleTimeBuffer::~SingleTimeBuffer() {
	if(!wasEnded)
		endSingleTimeBuffer();
}

void SingleTimeBuffer::endSingleTimeBuffer() {
	if (!wasEnded) {
		wasEnded = true;
		vkEndCommandBuffer(commandBuffer->getRaw());
		logicalDevice->queueSubmitForSingleBuffer(commandBuffer);
		logicalDevice->queueWaitIdleGraphics();
		vkFreeCommandBuffers(logicalDevice->getRaw(), commandPool->getRaw(), 1, &commandBuffer->getRaw());
	}
}

CommandBuffer* SingleTimeBuffer::getCommandBuffer() {
	return commandBuffer;
}