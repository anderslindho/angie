#pragma once

#include <chrono>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

struct Application {
private:
  GLFWwindow *m_window;
  unsigned int m_program;
  unsigned int VBO, VAO, EBO;
  std::chrono::time_point<std::chrono::system_clock> m_start_time;

public:
  Application(const int width, const int height);
  ~Application();

  void initialise();
  void run();
};