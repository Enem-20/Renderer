#include "BaseRenderer.h"

#include "WindowManager.h"
#include "Window.h"

#include "Resources/ResourceManager.h"

#include <GLFW/glfw3.h>

bool BaseRenderer::windowShouldClose() const{
	return glfwWindowShouldClose(WindowManager::GetCurrentWindow()->GetRaw());
}



