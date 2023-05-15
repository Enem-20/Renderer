#include "DescriptionSets.h"

#include "UniformBuffer.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"
#include "../Texture2D.h"
#include "LogicalDevice.h"
#include "GeneralVulkanStorage.h"
#include "../UniformBufferObject.h"

#include "../../src/Resources/ResourceManager.h"

#include <array>
#include <iostream>

DescriptionSets::DescriptionSets(const std::string& name, LogicalDevice& logicalDevice, DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool, UniformBuffers& uniformBuffers, const std::vector<std::shared_ptr<Texture2D>>& textures)
	: descriptorSetLayout(descriptorSetLayout)
	, ResourceBase(name)
{
	std::vector<VkDescriptorSetLayout> layouts(GeneralVulkanStorage::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout.getDescriptorSetLayout());
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
		bufferInfo.buffer = uniformBuffers.getRaw()[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		std::vector<VkDescriptorImageInfo> imageInfos;
		imageInfos.resize(textures.size());

		for (size_t i = 0; i < imageInfos.size(); ++i) {
			imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfos[i].imageView = textures[i]->getImageView();
			imageInfos[i].sampler = textures[i]->getTextureSampler();
		}	

		std::array<VkWriteDescriptorSet, 2> descriptorWrites;

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		//descriptorWrites[0].pImageInfo = nullptr;
		//descriptorWrites[0].pTexelBufferView = nullptr;
		descriptorWrites[0].pNext = nullptr;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = imageInfos.data();
		descriptorWrites[1].pBufferInfo = nullptr;
		//descriptorWrites[1].pTexelBufferView = nullptr;
		descriptorWrites[1].pNext = nullptr;
		auto trier = static_cast<uint32_t>(descriptorWrites.size());

		vkUpdateDescriptorSets(logicalDevice.getRaw(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

	ResourceManager::addResource<DescriptionSets>(this);
}

std::vector<VkDescriptorSet>& DescriptionSets::getDescriptorSets() {
	return descriptorSets;
}