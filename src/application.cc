#include "application.hh"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>

#include "shader.hh"
#include "utils.hh"

Application::Application(const unsigned int width, const unsigned int height) {
  m_start_time = std::chrono::system_clock::now();
  m_window = [](const int w, const int h) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto window = glfwCreateWindow(w, h, "angie", nullptr, nullptr);
    if (!window) {
      spdlog::error("Failed to init window");
      glfwTerminate();
      std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
      spdlog::error("Failed on init glad");
      glfwTerminate();
      glfwDestroyWindow(window);
      std::exit(EXIT_FAILURE);
    }
    return window;
  }(width, height);
}

Application::~Application() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  if (m_window)
    glfwDestroyWindow(m_window);
  glfwTerminate();

  const auto current_time = std::chrono::system_clock::now();
  const auto run_time = std::chrono::duration_cast<std::chrono::seconds>(
                            current_time - m_start_time)
                            .count();
  spdlog::info("Program ran for {} seconds.", run_time);
}

void Application::initialise() {
  float vertices[] = {
      // positions         // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void Application::run() {
  auto program = Shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
  while (!glfwWindowShouldClose(m_window)) {
    process_input(m_window);

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program.use();
    const auto current_time = std::chrono::system_clock::now();
    const auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                              current_time - m_start_time)
                              .count();

    const auto sin = std::sin(run_time / 1000.0f) / 2.0f + 0.5f;
    program.set_vec3("ourColour", 1.0f, sin, 0.3f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}
