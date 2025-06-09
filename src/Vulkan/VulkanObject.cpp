#include "VulkanObject.h"

VulkanObject::VulkanObject(VulkanObject* parent)
    : parent(parent)
{

}
VulkanObject::~VulkanObject() {
    for (auto child : children) {
        delete child;
    }
    children.clear();
}