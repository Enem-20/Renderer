#pragma once

#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class Window;
class UIelement;

class WindowManager
{
public:
	static int init(std::string name, int width = 1080, int height = 1080);
	static void ShutDown();

	static std::shared_ptr<Window> GetWindow(std::string name);
	static std::shared_ptr<Window> AddWindow(std::string name, int width = 1080, int height = 1080);
	static Window& GetCurrentWindow();
	static std::shared_ptr<Window> CurrentWindow;
	static void Start();
	static void Update();

	template<class T>
	static std::vector<std::shared_ptr<UIelement>> getUIelementVectorByWindowName(const std::string& windowName);
	template<class T>
	static void addUIelementByWindowName(const std::string& windowName, const std::shared_ptr<UIelement>& uiElement);
private:
	static std::unordered_map<std::string, std::shared_ptr<Window>> windows;
	static std::unordered_map<std::string, std::vector<std::shared_ptr<UIelement>>> UIelementVectorByWindowName;
};

#endif //WINDOW_MANAGER