#include "application.hh"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <glbinding/glbinding.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "renderer.hh"
#include "shader.hh"

Application::Application(const unsigned int width, const unsigned int height,
                         const std::string &title)
    : // m_width(width), m_height(height),
      m_title(title) {

  m_window = [](const int w, const int h, const std::string &t) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
    auto window = glfwCreateWindow(w, h, t.c_str(), nullptr, nullptr);
    if (!window) {
      glfwTerminate();
      throw std::runtime_error("Failed to init window");
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
  }(width, height, title);
  m_renderer = std::make_unique<Renderer>();
  // m_camera = std::make_unique<Camera>();
  m_start_time = std::chrono::system_clock::now();
}

void Application::run() const {
  // TODO: move out
  struct Model {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::string texture;
  };
  struct Model square {
    {
        // positions        // colors         // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    },
        {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        },
    {
      "container.jpg"
    }
  };
  Shader program("basic.vert", "basic.frag");

  auto mesh = std::make_unique<Mesh>(square.vertices, square.indices);
  mesh->add_texture(square.texture);

  while (!glfwWindowShouldClose(m_window)) {
    m_renderer->prepare();

    {
      program.use();
      const auto run_time =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now() - m_start_time)
              .count();
      const auto wave = std::sin(run_time / 1000.f) / 2.5f + 0.6f;
      program.set_vec3("u_modifier", glm::vec3(wave, wave, wave));

      glm::mat4 identity = glm::mat4(1.0f);
      glm::mat4 model =
          glm::rotate(identity, glm::radians(-55.0f * run_time / 1000.f),
                      glm::vec3(1.0f, 0.0f, 0.0f));
      glm::mat4 view = glm::translate(identity, glm::vec3(0.0f, 0.0f, -3.0f));
      glm::mat4 projection =
          glm::perspective(glm::radians(45.0f), 800.0f / 640.0f, 0.1f, 100.0f);
      program.set_mat4("u_model", model);
      program.set_mat4("u_view", view);
      program.set_mat4("u_projection", projection);
    }

    m_renderer->render(mesh, program);
    process_input(m_window);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }
}

Application::~Application() {
  if (m_window)
    glfwDestroyWindow(m_window);
  glfwTerminate();

  const auto run_time = std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now() - m_start_time)
                            .count();
  spdlog::info("Program ran for {} seconds.", run_time);
}
