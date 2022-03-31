#include "application.hh"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>

#include "utils.hh"

Application::Application(const int width, const int height) {
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
    const GLchar *vertex_shader_source_C = vertex_shader_src.c_str();
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
    const GLchar *fragment_shader_src_C = fragment_shader_src.c_str();
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
  const float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
  };
  const unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void Application::run() {
  while (!glfwWindowShouldClose(m_window)) {
    process_input(m_window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}
