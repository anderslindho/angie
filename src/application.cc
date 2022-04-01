#include "application.hh"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>

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
  m_program = []() {
    int success;
    char info_log[512];

    const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const std::string vertex_shader_src = read_file("res/shaders/vertex.glsl");
    const char *vertex_shader_source_C = vertex_shader_src.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_source_C, nullptr);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
      spdlog::error("Failed to compile vertex shader", info_log);
    }

    const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const std::string fragment_shader_src =
        read_file("res/shaders/fragment.glsl");
    const char *fragment_shader_src_C = fragment_shader_src.c_str();
    glShaderSource(fragment_shader, 1, &fragment_shader_src_C, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
      spdlog::error("Failed to compile fragment shader {}", info_log);
    }

    const auto shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
      spdlog::error("Failed to link shader program\n{}", info_log);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
  }();
}

Application::~Application() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glDeleteProgram(m_program);

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
  while (!glfwWindowShouldClose(m_window)) {
    process_input(m_window);

    glClearColor(k_bg_colour[0], k_bg_colour[1], k_bg_colour[2],
                 k_bg_colour[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    const auto current_time = std::chrono::system_clock::now();
    const auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                              current_time - m_start_time)
                              .count();

    const auto sin = std::sin(run_time / 1000.0f) / 2.0f + 0.5f;
    const auto vertex_location = glGetUniformLocation(m_program, "ourColour");
    glUseProgram(m_program);
    glUniform4f(vertex_location, 1.0, sin, 0.3, 1.0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}
