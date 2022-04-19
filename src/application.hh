#pragma once

#include <chrono>

#include <GLFW/glfw3.h>

#include "render.hh"

struct Application {
private:
  GLFWwindow *m_window;
  std::unique_ptr<Renderer> m_renderer;
  std::chrono::time_point<std::chrono::system_clock> m_start_time;

public:
  Application(const unsigned int width, const unsigned int height);
  ~Application();

  void run() const;
};
