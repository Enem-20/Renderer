#pragma once

#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

#include "API/ExportPropety.h"

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class Window;
class UIelement;

class DLLEXPORT WindowManager
{
public:
	static int init(std::string name, int width = 1080, int height = 1080);
	static void ShutDown();

	static std::shared_ptr<Window> GetWindow(std::string name);
	static std::shared_ptr<Window> AddWindow(std::string name, int width = 1080, int height = 1080);
	static std::shared_ptr<Window> GetCurrentWindow();
	static std::shared_ptr<Window> CurrentWindow;
	static void Start(uint32_t currentFrame);
	static void Update(uint32_t currentFrame);

	template<class T>
	static std::vector<std::shared_ptr<UIelement>> getUIelementVectorByWindowName(const std::string& windowName);
	template<class T>
	static void addUIelementByWindowName(const std::string& windowName, const std::shared_ptr<UIelement>& uiElement);
private:
	static std::unordered_map<std::string, std::shared_ptr<Window>> windows;
	static std::unordered_map<std::string, std::vector<std::shared_ptr<UIelement>>> UIelementVectorByWindowName;
};

#endif //WINDOW_MANAGER