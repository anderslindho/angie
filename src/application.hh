#pragma once

#include <chrono>

#include <GLFW/glfw3.h>

#include "render.hh"

struct Application {
private:
  GLFWwindow *m_window;
  // std::unique_ptr<Camera> m_camera;
  std::unique_ptr<Renderer> m_renderer;
  std::chrono::time_point<std::chrono::system_clock> m_start_time;

public:
  Application(const unsigned int width, const unsigned int height);
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;
  Application(Application &&) = delete;
  Application &operator=(Application &&) = delete;
  ~Application();

  void run() const;
};
