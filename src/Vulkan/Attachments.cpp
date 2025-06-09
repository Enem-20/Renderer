#include "Attachements.h"
#include "Attachment.h"

Attachments::Attachments(VkFormat format, size_t colorSize, size_t depthSize, size_t colorResolveSize)
    : attachements(colorSize + depthSize + colorResolveSize)
    , colorSize(colorSize)
    , colorOffset(0)
    , depthSize(depthSize)
    , depthOffset(0)
    , colorResolveSize(colorResolveSize) 
    , colorResolveOffset(0)
{

}

void Attachments::addColorAttachement(size_t samplesCount) {
    attachements.insert(attachements.begin() + colorOffset, new Attachment(format, samplesCount, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0));
    colorOffset += 1;
    depthOffset += 1;
    colorResolveOffset += 1;
}

void Attachments::addDepthAttachement(size_t samplesCount) {
	attachements.insert(attachements.begin() + depthOffset, new Attachment(format, samplesCount, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1));
    depthOffset += 1;
    colorResolveOffset += 1;
}

void Attachments::addColorResolveAttachement(size_t samplesCount) {
	attachements.insert(attachements.begin() + colorResolveOffset, new Attachment(format, samplesCount, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 2));
    colorResolveOffset += 1;
}