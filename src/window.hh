#pragma once

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height) {
  spdlog::info("width: {}, height: {}", width, height);
  gl::glViewport(0, 0, width, height);
}

class Window {
private:
  GLFWwindow *m_window;

public:
  Window(const int width, const int height) {
    m_window = [](const int w, const int h) {
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
      auto window = glfwCreateWindow(w, h, "angie", nullptr, nullptr);
      if (!window) {
        spdlog::error("Failed to init window");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
      }
      spdlog::info("GLFW {}", glfwGetVersionString());
      glfwMakeContextCurrent(window);

      /* necessary to redefine the (global) callbacks
         retrieve with
         reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)); see
         https://stackoverflow.com/questions/27387040/referencing-glfws-callback-functions-from-a-class
       */
      // glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

      glbinding::initialize(glfwGetProcAddress);
      auto gl_version = std::string(
          reinterpret_cast<const char *>(gl::glGetString(gl::GL_VERSION)));
      spdlog::info("OpenGL {}", gl_version);

      return window;
    }(width, height);
  }
  ~Window() {
    if (m_window)
      glfwDestroyWindow(m_window);
    glfwTerminate();
  }
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  void update() const {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
  bool should_stay_open() const { return !glfwWindowShouldClose(m_window); }

  void process_input(Camera &camera) {
    const auto window_ptr = get_window_ptr();

    if (glfwGetKey(window_ptr, GLFW_KEY_W) == GLFW_PRESS)
      camera.move(Direction::k_forward);
    if (glfwGetKey(window_ptr, GLFW_KEY_S) == GLFW_PRESS)
      camera.move(Direction::k_backward);

    if (glfwGetKey(window_ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window_ptr, true);
  }

private:
  GLFWwindow *get_window_ptr() { return m_window; }
};
