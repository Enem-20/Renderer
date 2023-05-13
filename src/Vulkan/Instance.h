#pragma once

#include "../../src/ExportPropety.h"
#include "../../src/Resources/ResourceBase.h"

#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>
#include <iostream>
#include <functional>
#include <optional>
#include <memory>

class Instance : public ResourceBase{
public:
	Instance(const std::string& name);
	~Instance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	std::vector<const char*> getRequiredExtensions() const;
	bool checkValidationLayerSupport() const;
	VkInstance& getRaw();

	inline static const std::string type = GETTYPE(Instance);
private:
	VkInstance instance;
};