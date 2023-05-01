#pragma once

#include "Interface.h"
#include "Capturer.h"


#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>
#include <iostream>
#include <functional>
#include <optional>
#include <memory>

class Instance {
public:
	Instance();
	~Instance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	std::vector<const char*> getRequiredExtensions() const;
	bool checkValidationLayerSupport() const;
	VkInstance& getRaw();
private:
	VkInstance instance;
};