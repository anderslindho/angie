#pragma once

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

class Window {
private:
  GLFWwindow *m_window;

public:
  Window(const int width, const int height);
  ~Window();
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  void update() const;
  bool should_stay_open() const { return !glfwWindowShouldClose(m_window); }
  void close() { glfwSetWindowShouldClose(m_window, true); }

  bool is_key_pressed(int key);
  void get_cursor_position(double &x, double &y) const;

  void set_resize_callback(std::function<void(int, int)> callback);

private:
  std::function<void(int, int)> m_resize_callback;

  void set_wireframe_mode(bool state);

  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
};
