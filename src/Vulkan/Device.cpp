#include "Device.h"
#include "PhysicalDeviceSelector.h"

Device::Device(std::string_view name, Instance* instance, VkPhysicalDevice physicalDevice) 
    : device(std::string("PhysicalDevice") + name.data(), physicalDevice, this, {"VK_KHR_swapchain"})
    , logicalDevice(std::string("LogicalDevice") + name.data(), this)
    , instance(instance)
{

}