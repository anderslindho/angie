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

#include "camera/cameracontroller.hh"
#include "graphics/shader.hh"
#include "window.hh"

Application::Application(const unsigned int width, const unsigned int height,
                         const std::string &title)
    : m_title(title), m_start_time(std::chrono::system_clock::now()),
      m_camera(std::make_unique<Camera>(0.f, 0.f, 0.f)),
      m_window(std::make_unique<Window>(width, height)),
      m_renderer(std::make_unique<Renderer>()),
      m_camera_controller(
          std::make_unique<CameraController>(*m_camera, *m_window)) {}

void Application::run() const {
  struct Model {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::string texture;
  };

  // These coordinates and indices are shamelessly stolen
  struct Model cube {
    {
        -0.5f, -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
        0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
        0.0f,  1.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,
        -0.5f, 1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,
        0.0f,  1.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,
        -0.5f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 1.0f,
        0.0f,  0.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
        1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, 0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f,
        -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,
        1.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.5f,  0.5f,
        0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
        0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  1.0f,  0.0f,
        0.0f,  1.0f,
    },
        {
            0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
            8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
        },
    {
      "container.jpg"
    }
  };

  auto prev_time = std::chrono::system_clock::now();

  auto mesh = std::make_unique<Mesh>(cube.vertices, cube.indices);
  mesh->add_texture(cube.texture);

  Shader program("basic.vert", "basic.frag");

  std::vector<glm::vec3> cube_positions = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  program.use();
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), 800.0f / 640.0f, 0.1f, 100.0f);
  program.set_mat4("u_projection", projection);

  while (m_window->should_stay_open()) {
    auto time = std::chrono::system_clock::now();
    const float delta_time =
        std::chrono::duration<float>(time - prev_time).count();

    m_renderer->prepare();
    for (auto ele : cube_positions) {
      program.set_mat4("u_model", glm::translate(glm::mat4{1.f}, ele));
      m_renderer->render(mesh, program);
    }

    auto view = m_camera->get_view_matrix();
    program.set_mat4("u_view", view);

    m_camera_controller->handle_input(delta_time);
    m_window->update();

    prev_time = time;
  }
}

Application::~Application() {
  const auto run_time = std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::system_clock::now() - m_start_time)
                            .count();
  spdlog::info("Program ran for {} seconds.", run_time);
}
