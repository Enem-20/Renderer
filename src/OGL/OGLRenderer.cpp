#include "OGLRenderer.h"

#include "../WindowManager.h"
#include "Resources/ResourceManager.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

OGLRenderer::OGLRenderer(const std::string& name) : BaseRenderer(name) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__) && defined(__MACH__) 
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // defined(__APPLE__) && defined(__MACH__) 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	WindowManager::init("OpenGL", 600, 600);

	std::function<void(GLFWwindow* window, int width, int height)> frameBufferSizeCallback = [&](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	};

	glfwSetFramebufferSizeCallback(WindowManager::CurrentWindow->GetRaw(), frameBufferSizeCallback.target<void(GLFWwindow * window, int width, int height)>());

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	ResourceManager::addResource<OGLRenderer>(this);
}

OGLRenderer::~OGLRenderer() {
	glfwTerminate();
}

void OGLRenderer::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	//use shaders here

	//bind buffers here

	//set active vertexes here

	drawFrame();

	//disable vertexes here
	glfwSwapBuffers(WindowManager::GetCurrentWindow()->GetRaw());
	glfwPollEvents();
}
void OGLRenderer::drawFrame() {
	//glDrawElements(GL_TRIANGLES,);
}

void OGLRenderer::initWindow() {

}

std::shared_ptr<BaseTexture2D> OGLRenderer::createTexture(std::string_view name, const std::string& relativePath, const uint32_t& texWidth, const uint32_t& texHeight, unsigned char* pixels, const uint32_t& texChannels) {
	BaseTexture2D* texture = new OGLTexture2D(name, relativePath, texWidth, texHeight, pixels, texChannels, std::unordered_map<size_t, std::string_view>{});
	ResourceManager::addResource<BaseTexture2D>(texture);
	
	return textures.emplace_back(ResourceManager::getResource<BaseTexture2D>(name));
}

void OGLRenderer::addTexture(std::shared_ptr<BaseTexture2D> texture) {
	textures.push_back(texture);
}
void OGLRenderer::removeTexture(std::string_view name) {
	std::remove_if(textures.begin(), textures.end(), [name](std::shared_ptr<BaseTexture2D> texture) {return (texture->name == name) ? true : false; });
}

void OGLRenderer::OnBeforeFrame() {

}

void OGLRenderer::setViewport(int width, int height, int leftOffset, int bottomOffset) {
	glViewport(leftOffset, bottomOffset, width, height);
}

void OGLRenderer::afterComplete() {

}