#ifndef C_DEVICE_H
#define C_DEVICE_H

#include <GLFW/glfw3.h>

#include "API/ExportPropety.h"

#include "Instance.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "WindowSurface.h"


class DLLEXPORT Device {
public:
    Device(std::string_view name, Instance* instance, VkPhysicalDevice physicalDevice);
    PhysicalDevice device;
    LogicalDevice logicalDevice;
    Instance* instance = nullptr;
};

#endif