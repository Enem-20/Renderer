#pragma once

#ifndef VULKAN_SHADERPROGRAM
#define VULKAN_SHADERPROGRAM
#include "API/ExportPropety.h"
#include "../BaseShaderProgram.h"

#include <glm/mat4x4.hpp>

#include <string>

struct VkShaderModule_T;
typedef VkShaderModule_T* VkShaderModule;


class DLLEXPORT VulkanShaderProgram : public BaseShaderProgram
{

public:
	VulkanShaderProgram(std::string_view name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& vertexShader, const std::string& fragmentShader);
	~VulkanShaderProgram();

	VulkanShaderProgram() = delete;

	VkShaderModule& getVertexShaderModule();
	VkShaderModule& getFragmentShaderModule();

	std::string vertexShaderPath;
	std::string fragmentShaderPath;

	GENERATETYPE(VulkanShaderProgram)
private:
	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;
	bool createShaders(const std::string& vertexShader, const std::string& fragmentShader);
	VkShaderModule createShader(const std::string& source);
};

#endif // !VULKAN_SHADERPROGRAM