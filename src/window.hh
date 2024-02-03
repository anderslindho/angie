#pragma once

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

#include "camera.hh"

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height);

void mouse_callback(GLFWwindow * /* window */, double x_pos, double y_pos);

class Window {
private:
  GLFWwindow *m_window;
  Camera &m_camera;

public:
  Window(const int width, const int height, Camera &camera);
  ~Window();
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  void update() const;
  bool should_stay_open() const { return !glfwWindowShouldClose(m_window); }
  void process_keyboard_input(float delta_time);
  void process_mouse_movement(float delta_time);

private:
  GLFWwindow *get_window_ptr() { return m_window; }
};
