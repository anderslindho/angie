#pragma once

#include <chrono>

#include <GLFW/glfw3.h>
#include <glad/glad.h>


struct Application {
private:
    GLFWwindow *m_window;
    int m_width;
    int m_height;
    std::chrono::time_point<std::chrono::system_clock> m_start_time;
public:
    Application(const int width, const int height);
    ~Application();
};