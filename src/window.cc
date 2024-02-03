#include "window.hh"

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height) {
  spdlog::debug("width: {}, height: {}", width, height);
  gl::glViewport(0, 0, width, height);
}

Window::Window(const int width, const int height, Camera &camera)
    : m_camera(camera) {
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

    gl::glEnable(gl::GL_DEPTH_TEST);

    return window;
  }(width, height);
}

Window::~Window() {
  if (m_window)
    glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::update() const {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

void Window::process_keyboard_input(float delta_time) {
  const auto window_ptr = get_window_ptr();

  if (glfwGetKey(window_ptr, GLFW_KEY_W) == GLFW_PRESS)
    m_camera.move(Direction::k_forward, delta_time);
  if (glfwGetKey(window_ptr, GLFW_KEY_S) == GLFW_PRESS)
    m_camera.move(Direction::k_backward, delta_time);
  if (glfwGetKey(window_ptr, GLFW_KEY_A) == GLFW_PRESS)
    m_camera.move(Direction::k_left, delta_time);
  if (glfwGetKey(window_ptr, GLFW_KEY_D) == GLFW_PRESS)
    m_camera.move(Direction::k_right, delta_time);
  if (glfwGetKey(window_ptr, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_camera.move(Direction::k_up, delta_time);
  if (glfwGetKey(window_ptr, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    m_camera.move(Direction::k_down, delta_time);

  if (glfwGetKey(window_ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window_ptr, true);
}

void Window::process_mouse_movement(float delta_time) {
  const auto window_ptr = get_window_ptr();

  double x_pos, y_pos;
  glfwGetCursorPos(window_ptr, &x_pos, &y_pos);
  m_camera.handle_mouse_movement(x_pos, y_pos, delta_time);
}
