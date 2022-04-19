#include "application.hh"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <spdlog/spdlog.h>

#include <glbinding/glbinding.h>

#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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
    auto gl_version = std::string(reinterpret_cast<const char*>(gl::glGetString(gl::GL_VERSION)));
    spdlog::info("OpenGL {}", gl_version);

    return window;
  }(width, height);
}

void Application::initialise() {
  float vertices[] = {
      // positions        // colors         // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  int image_width, image_height, image_channel_count;
  std::string image = "res/textures/container.jpg";
  void *image_data = stbi_load(image.c_str(), &image_width, &image_height,
                               &image_channel_count, 0);
  if (!image_data)
    spdlog::error("Failed to load image {}", image);

  gl::glGenVertexArrays(1, &VAO);
  gl::glGenBuffers(1, &VBO);
  gl::glGenBuffers(1, &EBO);
  gl::glGenTextures(1, &TBO);

  gl::glBindVertexArray(VAO);

  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, VBO);
  gl::glBufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, gl::GL_STATIC_DRAW);

  gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, EBO);
  gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               gl::GL_STATIC_DRAW);

  gl::glBindTexture(gl::GL_TEXTURE_2D, TBO);
  gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGB, image_width, image_height, 0, gl::GL_RGB,
               gl::GL_UNSIGNED_BYTE, image_data);
  gl::glGenerateMipmap(gl::GL_TEXTURE_2D);

  // position attribute
  gl::glVertexAttribPointer(0, 3, gl::GL_FLOAT, gl::GL_FALSE, 8 * sizeof(float), (void *)0);
  gl::glEnableVertexAttribArray(0);
  // color attribute
  gl::glVertexAttribPointer(1, 3, gl::GL_FLOAT, gl::GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  gl::glEnableVertexAttribArray(1);
  // texture attribute
  gl::glVertexAttribPointer(2, 2, gl::GL_FLOAT, gl::GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  gl::glEnableVertexAttribArray(2);

  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, 0);

  stbi_image_free(image_data);
}

void Application::run() {
  Shader program("res/shaders/basic.vert", "res/shaders/basic.frag");
  program.use();
  while (!glfwWindowShouldClose(m_window)) {
    process_input(m_window);

    gl::glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT);

    const auto current_time = std::chrono::system_clock::now();
    const auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                              current_time - m_start_time)
                              .count();

    const auto wave = std::sin(run_time / 1000.0f) / 2.5f + 0.6f;
    program.set_vec3("u_modifier", glm::vec3(wave, wave, wave));

    gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(m_window);
    glfwPollEvents();

    glCheckError();
  }
}

Application::~Application() {
  gl::glDeleteVertexArrays(1, &VAO);
  gl::glDeleteBuffers(1, &VBO);
  gl::glDeleteBuffers(1, &EBO);
  gl::glDeleteTextures(1, &TBO);

  if (m_window)
    glfwDestroyWindow(m_window);
  glfwTerminate();

  const auto current_time = std::chrono::system_clock::now();
  const auto run_time = std::chrono::duration_cast<std::chrono::seconds>(
                            current_time - m_start_time)
                            .count();
  spdlog::info("Program ran for {} seconds.", run_time);
}