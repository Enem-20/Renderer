#include "WindowManager.h"

#include "Window.h"
#include "../../UI/src/UIelement.h"

#include <../../../src/Resources/ResourceManager.h>

//#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

std::unordered_map<std::string, std::shared_ptr<Window>> WindowManager::windows = {};
std::shared_ptr<Window> WindowManager::CurrentWindow;

std::shared_ptr<Window> WindowManager::GetWindow(std::string name)
{
	std::shared_ptr<Window> window = windows.find(name)->second;
	return window ? window : nullptr;
}

int WindowManager::init(std::string name, int width, int height)
{
	CurrentWindow = ResourceManager::makeResource<Window>(name, width, height);

	glfwMakeContextCurrent(CurrentWindow->window);
	//if (!gladLoadGL())
	//{
	//	std::cout << "Error. glad is not initialized" << std::endl;
	//	glfwTerminate();
	//	system("pause");
	//	return -1;
	//}

	windows.emplace(name, CurrentWindow);

	return 0;
}

void WindowManager::ShutDown()
{
	windows.clear();
	glfwTerminate();
}
std::shared_ptr<Window> WindowManager::GetCurrentWindow()
{
	return CurrentWindow;
}

std::shared_ptr<Window> WindowManager::AddWindow(std::string name, int width, int height)
{
	if (CurrentWindow != nullptr)
	{
		CurrentWindow->CreateWindow(name, width, height);

		glfwMakeContextCurrent(CurrentWindow->window);
	}
	else
	{
		CurrentWindow = ResourceManager::makeResource<Window>(name, width, height);
		glfwMakeContextCurrent(CurrentWindow->window);
	}

	if (!CurrentWindow->window)
	{
		glfwTerminate();
		std::cout << "Window " << name <<  " is not created!" << std::endl;
		system("pause");
	}

	windows.emplace(name, CurrentWindow);

	return CurrentWindow;
}

void WindowManager::Start(uint32_t currentFrame)
{
	for (auto& window : windows)
	{
		window.second->Start(currentFrame);
	}
}

void WindowManager::Update(uint32_t currentFrame)
{
	for (auto& window : windows)
	{
		window.second->Update(currentFrame);
	}
}

template<class T>
std::vector<std::shared_ptr<UIelement>> WindowManager::getUIelementVectorByWindowName(const std::string& windowName) {
	auto UIelementVectorIT = UIelementVectorByWindowName.find(windowName);

	if (UIelementVectorIT != UIelementVectorByWindowName.end()) {
		return UIelementVectorIT->second;
	}

	return std::vector<std::shared_ptr<UIelement>>();
}

template<class T>
void WindowManager::addUIelementByWindowName(const std::string& windowName, const std::shared_ptr<UIelement>& uiElement) {
	if (UIelementVectorByWindowName.contains(windowName)) {
		auto UIelementVectorIT = UIelementVectorByWindowName.find(windowName);

		UIelementVectorIT->second.push_back(uiElement);
	}

	UIelementVectorByWindowName.emplace(windowName, uiElement);
}