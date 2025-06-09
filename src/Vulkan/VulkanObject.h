#ifndef C_VULKAN_OBJECT_H
#define C_VULKAN_OBJECT_H

#include <vector>

class VulkanObject {
public:
    VulkanObject(VulkanObject* parent = nullptr);
    virtual ~VulkanObject();
protected:
    VulkanObject* parent;
    std::vector<VulkanObject*> children;
};

#endif