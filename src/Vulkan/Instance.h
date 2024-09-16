#pragma once

#ifndef INSTANCE_H
#define INSTANCE_H

#include "API/ExportPropety.h"
#include "Resources/ResourceBase.h"

#include <cstdint>
#include <vector>
#include <iostream>
#include <functional>
#include <optional>
#include <memory>

struct VkInstance_T;
typedef VkInstance_T* VkInstance;

struct VkDebugUtilsMessengerCreateInfoEXT;

class Instance : public ResourceBase{
public:
	Instance(const std::string& name);
	~Instance();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	std::vector<const char*> getRequiredExtensions() const;
	bool checkValidationLayerSupport() const;
	VkInstance& getRaw();

	GENERATETYPE(Instance)
private:
	VkInstance instance;
};

#endif