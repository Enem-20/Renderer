#include "OGLRenderBackend.h"

#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../WindowManager.h"
#include "../Window.h"
#include "../Renderable.h"

void OGLRenderBackend::Init(void* windowHandle) {
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
}

void OGLRenderBackend::Shutdown() {
    // очистка VAO/VBO, шейдеров, текстур
}

void BeginFrame() {
    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawMesh(const Renderable& renderUnit) {
    //renderUnit.GetMaterial()->shader->Bind();
    //material->shader->SetUniform("u_Model", modelMatrix);
    //material->ApplyTextures();
    //glBindVertexArray(mesh.GetVAO());
    //glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void EndFrame() {
    glfwSwapBuffers(WindowManager::GetCurrentWindow()->GetRaw());
	glfwPollEvents();
}