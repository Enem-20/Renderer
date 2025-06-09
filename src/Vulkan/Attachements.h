#ifndef S_ATTACHEMENTS_H
#define S_ATTACHEMENTS_H

#include <cstddef>
#include <vector>

#include <vulkan/vulkan.h>

class Attachment;

struct Attachments {
	Attachments() = default;
	~Attachments() = default;

    Attachments(VkFormat format, size_t colorSize, size_t depthSize, size_t colorResolveSize);

	void addColorAttachement(size_t samplesCount = 0);
	void addDepthAttachement(size_t samplesCount = 0);
	void addColorResolveAttachement(size_t samplesCount = 0);
private:
    VkFormat format = VK_FORMAT_UNDEFINED;
	std::vector<Attachment*> attachements;
    size_t colorSize = 0;
    size_t colorOffset = 0;

    size_t depthSize = 0;
    size_t depthOffset = 0;

    size_t colorResolveSize = 0;
    size_t colorResolveOffset = 0;
};

#endif