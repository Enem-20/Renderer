#pragma once

#ifndef WINDOW
#define WINDOW

#include "../../src/Resources/ResourceBase.h"

#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include <memory>
#include <unordered_map>
#include <string>


struct GLFWwindow;
class UIelement;
class Panel;
//class UIelement;

//#define OGL

class Window : public ResourceBase
{
#if defined(OGL) || defined(GLFW_INCLUDE_VULKAN)
public:
	Window(const std::string& name);
	~Window();
	Window(const std::string& name, GLFWwindow* window);
	Window(const std::string& name, int width, int height);
	GLFWwindow* GetRaw();
	void SetWindow(GLFWwindow* window);
	std::shared_ptr<Window> CreateWindow(const std::string& name, const unsigned int& width, const unsigned int& height);
	void Awake(uint32_t currentFrame);
	void Start(uint32_t currentFrame);
	void Update(uint32_t currentFrame);
	void FixedUpdate();

	std::shared_ptr<Panel>& AddPanel(const std::shared_ptr<Panel>& ui);
	std::shared_ptr<Panel> GetPanel(const std::string& name) const;
	void RemovePanel(const std::string name);

	GLFWwindow* window;
	glm::ivec2 size;
	std::unordered_map<std::string, std::shared_ptr<Panel>> panels;

	GENERATETYPE(Window)
#endif

};

#endif //WINDOW