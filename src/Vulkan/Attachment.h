#ifndef S_ATTACHEMENT_H
#define S_ATTACHEMENT_H

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

struct Attachment {
    Attachment(VkFormat format, size_t samplesCount, VkImageLayout layout, size_t layoutIndex = 0) 
        : index(layoutIndex)
    {
        for(size_t i = 0; i < attachments.size(); ++i) {
            VkAttachmentDescription attachment{};
            VkAttachmentReference attachmentRef{};
        
            attachment.format = format;
            if (layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
                attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            else {
                if (samplesCount == 0)
                    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
                else {
                    switch (samplesCount) {
                        case 1:
                            attachment.samples = VK_SAMPLE_COUNT_1_BIT;
                            break;
                        default:
                            if ((samplesCount < 65) && (samplesCount % 2 == 0)) {
                                attachment.samples = static_cast<VkSampleCountFlagBits>(samplesCount);
                            } else {
                                throw std::runtime_error("Invalid sample count for attachment: " + std::to_string(samplesCount));
                            }
                    }
                }
            }

            attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachment.finalLayout = layout;
            attachments.push_back(attachment);

            attachmentRef.attachment = static_cast<uint32_t>(layoutIndex);
            if (layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
                attachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            else
                attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            attachmentRefs.push_back(attachmentRef);
        }
        
    }

    const std::vector<VkAttachmentDescription>& getAttachments() const {
        return attachments;
    }
    const std::vector<VkAttachmentReference>& getAttachmentRefs() const {
        return attachmentRefs;
    }

private:
    std::vector<VkAttachmentDescription> attachments;
	std::vector<VkAttachmentReference> attachmentRefs;
    size_t index{0};
};

#endif