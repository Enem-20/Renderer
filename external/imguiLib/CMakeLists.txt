# extern/imgui/CMakeLists.txt

# 1. Собираем исходники ImGui core
file(GLOB IMGUI_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp
)

# 2. Явно указываем нужные backends
set(IMGUI_BACKENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp
)

# 3. Создаём статическую библиотеку
add_library(imgui STATIC
    ${IMGUI_CORE}
    ${IMGUI_BACKENDS}
)

# 4. Добавляем include директории:
# - imgui/
# - imgui/backends/
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)

# 5. Линкуем с зависимостями (только то, что реально нужно)
# Vulkan — нужен всегда
# glfw — нужен для imgui_impl_glfw
# glad — нужен только если ты используешь OpenGL backend
target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)

# 6. Если ты используешь OpenGL и glad
# и glad — это твой внутренний target, например glad::glad
# или ты хочешь явно указать его
# добавь так:
# target_link_libraries(imgui PUBLIC glad)
