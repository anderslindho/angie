#pragma once

#include <chrono>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

const auto k_bg_colour = glm::vec4(0.1f, 0.1f, 0.2f, 1.0f);

struct Application {
private:
  GLFWwindow *m_window;
  unsigned int m_program;
  unsigned int VBO, VAO, EBO;
  std::chrono::time_point<std::chrono::system_clock> m_start_time;

public:
  Application(const unsigned int width, const unsigned int height);
  ~Application();

  void initialise();
  void run();
};
