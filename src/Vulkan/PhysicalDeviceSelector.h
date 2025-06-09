#ifndef C_PHYSICAL_DEVICE_SELECTOR_H
#define C_PHYSICAL_DEVICE_SELECTOR_H

#include <cstddef>

#include <tsl/hopscotch_map.h>

#include "SwapChain.h"

class PhysicalDevice;
class Instance;
class WindowSurface;

class PhysicalDeviceSelector {
public:
    static PhysicalDeviceSelector* getInstance();
public:
    void collectRenderDevices(Instance* instance, WindowSurface* surface);
    SwapChain::SwapChainSupportDetails querySwapChainSupport(PhysicalDevice* device, WindowSurface* windowSurface) const;
    bool checkDeviceExtensionSupport(PhysicalDevice* device) const;
    bool isDeviceSuitable(PhysicalDevice* device, WindowSurface* windowSurface) const;
    size_t getMaxUsableSampleCount(PhysicalDevice* device) const;
private:
    PhysicalDeviceSelector() = default;
    PhysicalDeviceSelector(const PhysicalDeviceSelector&) = delete;
    PhysicalDeviceSelector& operator=(const PhysicalDeviceSelector&) = delete;
    PhysicalDeviceSelector(PhysicalDeviceSelector&&) = delete;
    PhysicalDeviceSelector& operator=(PhysicalDeviceSelector&&) = delete;
private:
    static PhysicalDeviceSelector* selectorInstance;
    static const std::vector<const char*> deviceExtensions;
};

#endif