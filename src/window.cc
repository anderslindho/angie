#include "window.hh"

#include <spdlog/spdlog.h>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>

Window::Window(const int width, const int height, std::string title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
  auto window =
      glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!window) {
    spdlog::error("Failed to init window");
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }
  spdlog::info("GLFW {}", glfwGetVersionString());
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  /* necessary to redefine the (global) callbacks
    retrieve with
    reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)); see
    https://stackoverflow.com/questions/27387040/referencing-glfws-callback-functions-from-a-class
  */
  glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

  glbinding::initialize(glfwGetProcAddress);
  auto gl_version = std::string(
      reinterpret_cast<const char *>(gl::glGetString(gl::GL_VERSION)));
  spdlog::info("OpenGL {}", gl_version);

  gl::glEnable(gl::GL_DEPTH_TEST);

  m_window = window;
}

Window::~Window() {
  if (m_window)
    glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::update() const {
  glfwSwapBuffers(m_window);
  // TODO: replace with some sort of eventmanager
  glfwWaitEventsTimeout(refresh_timeout);
}

bool Window::is_key_pressed(int key) {
  // TODO: move elsewhere (input manager might be required)
  if (glfwGetKey(m_window, GLFW_KEY_N) == GLFW_PRESS) {
    set_wireframe_mode(true);
  }
  if (glfwGetKey(m_window, GLFW_KEY_M) == GLFW_PRESS) {
    set_wireframe_mode(false);
  }
  return glfwGetKey(m_window, key) == GLFW_PRESS;
}

void Window::get_cursor_position(double &x, double &y) const {
  glfwGetCursorPos(m_window, &x, &y);
}

void Window::set_resize_callback(std::function<void(int, int)> callback) {
  m_resize_callback = callback;
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}

void Window::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
  spdlog::debug("width: {}, height: {}", width, height);

  gl::glViewport(0, 0, width, height);
  Window *instance = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (instance != nullptr && instance->m_resize_callback) {
    instance->m_resize_callback(width, height);
  }
}

void Window::set_wireframe_mode(bool state) {
  if (state == true) {
    gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_LINE);
  } else {
    gl::glPolygonMode(gl::GL_FRONT_AND_BACK, gl::GL_FILL);
  }
}
