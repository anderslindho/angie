#include "application.hh"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <glbinding/glbinding.h>

#include "shader.hh"
#include "utils.hh"

Application::Application(const unsigned int width, const unsigned int height) {
  m_start_time = std::chrono::system_clock::now();
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glbinding::initialize(glfwGetProcAddress);
    auto gl_version = std::string(
        reinterpret_cast<const char *>(gl::glGetString(gl::GL_VERSION)));
    spdlog::info("OpenGL {}", gl_version);

    return window;
  }(width, height);
  m_renderer = new Renderer();
}

void Application::run() const {
  Shader program("res/shaders/basic.vert", "res/shaders/basic.frag");
  program.use();
  while (!glfwWindowShouldClose(m_window)) {
    process_input(m_window);

    m_renderer->prepare();

    const auto current_time = std::chrono::system_clock::now();
    const auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                              current_time - m_start_time)
                              .count();

    const auto wave = std::sin(run_time / 1000.0f) / 2.5f + 0.6f;
    program.set_vec3("u_modifier", glm::vec3(wave, wave, wave));

    m_renderer->render();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}

Application::~Application() {
  if (m_renderer)
    delete m_renderer;
  if (m_window)
    glfwDestroyWindow(m_window);
  glfwTerminate();

  const auto current_time = std::chrono::system_clock::now();
  const auto run_time = std::chrono::duration_cast<std::chrono::seconds>(
                            current_time - m_start_time)
                            .count();
  spdlog::info("Program ran for {} seconds.", run_time);
}
