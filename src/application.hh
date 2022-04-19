#pragma once

#include <chrono>

#include <GLFW/glfw3.h>


struct Application {
private:
  GLFWwindow *m_window;
  unsigned int VBO, VAO, TBO, EBO; // temporary members
  std::chrono::time_point<std::chrono::system_clock> m_start_time;

public:
  Application(const unsigned int width, const unsigned int height);
  ~Application();

  void initialise();
  void run();
};
