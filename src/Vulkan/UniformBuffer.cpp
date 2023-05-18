#include "UniformBuffer.h"

#include "../../../src/GameTypes/GameObject.h"
#include "../Sprite.h"
#include "../../src/Resources/ResourceManager.h"

#include "RenderPipeline.h"
#include "SwapChain.h"
#include "GeneralVulkanStorage.h"
#include "LogicalDevice.h"
#include "../UniformBufferObject.h"


#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


VkDescriptorSetLayout UniformBuffers::descriptorSetLayout;

UniformBuffers::UniformBuffers(const std::string& name, LogicalDevice& logicalDevice, SwapChain& swapchain)
	: logicalDevice(logicalDevice)
	, swapchain(swapchain)
	, ResourceBase(name)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT; ++i) {
		logicalDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(logicalDevice.getRaw(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}

	ResourceManager::addResource<UniformBuffers>(this);
}

UniformBuffers::~UniformBuffers() {
	ResourceManager::removeResource<UniformBuffers>(name);
	for (size_t i = 0; i < GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT; ++i) {
		vkDestroyBuffer(logicalDevice.getRaw(), uniformBuffers[i], nullptr);
		vkFreeMemory(logicalDevice.getRaw(), uniformBuffersMemory[i], nullptr);
	}
}

void UniformBuffers::bind(CommandBuffer& commandBuffer, RenderPipeline& renderPipeline, uint32_t currentFrame) {
	vkCmdBindDescriptorSets(commandBuffer.getRaw(), VK_PIPELINE_BIND_POINT_GRAPHICS, renderPipeline.getPipelineLayout(), 0, 1,
		&descriptorSets[currentFrame], 0, nullptr);
}

std::vector<VkBuffer>& UniformBuffers::getRaw() {
	return uniformBuffers;
}

VkDescriptorSetLayout& UniformBuffers::getDescriptorSetLayout() {
	return descriptorSetLayout;
}

void UniformBuffers::destroyDescriptorSetLayout(LogicalDevice& logicalDevice) {
	vkDestroyDescriptorSetLayout(logicalDevice.getRaw(), descriptorSetLayout, nullptr);
}

void UniformBuffers::updateUniformBuffer(uint32_t currentImage, UniformBufferObject ubo) {
		ubo.view = glm::mat4(1.0f);
		ubo.proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10000.0f, 10000.0f);
		ubo.proj[1][1] *= -1;

		memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));

	//UniformBufferObject ubo{};
	//ubo.model = glm::mat4(1.0f);
	//ubo.view = glm::mat4(1.0f);
	//ubo.proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10000.0f, 10000.0f);
	//ubo.proj[1][1] *= -1;

	//memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void UniformBuffers::createDescriptorSetLayout(LogicalDevice& logicalDevice) {
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;


	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(logicalDevice.getRaw(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void UniformBuffers::createDescriptorSets(DescriptorPool& descriptorPool) {;
	std::vector<VkDescriptorSetLayout> layouts(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool.getRaw();
	allocInfo.descriptorSetCount = static_cast<uint32_t>(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(logicalDevice.getRaw(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT; ++i) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite;

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		//descriptorWrites[0].pImageInfo = nullptr;
		//descriptorWrites[0].pTexelBufferView = nullptr;
		descriptorWrite.pNext = nullptr;

		vkUpdateDescriptorSets(logicalDevice.getRaw(), 1, &descriptorWrite, 0, nullptr);
	}
}
