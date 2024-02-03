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

#include "camera.hh"
#include "graphics/renderer.hh"
#include "graphics/shader.hh"
#include "window.hh"

Application::Application(const unsigned int width, const unsigned int height,
                         const std::string &title)
    : m_title(title) {

  m_window = std::make_unique<Window>(width, height);
  m_renderer = std::make_unique<Renderer>();
  m_camera = std::make_unique<Camera>(0.f, 0.f, 3.f);
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

  Shader program("basic.vert", "basic.frag");
  auto mesh = std::make_unique<Mesh>(cube.vertices, cube.indices);
  mesh->add_texture(cube.texture);

  auto prev_time = std::chrono::system_clock::now();

  while (m_window->should_stay_open()) {
    m_renderer->prepare();
    auto time = std::chrono::system_clock::now();
    const float delta_time =
        std::chrono::duration<float>(time - prev_time).count();

    {
      program.use();
      const auto run_time =
          std::chrono::duration_cast<std::chrono::milliseconds>(time -
                                                                m_start_time)
              .count();
      const auto wave = std::sin(run_time / 1000.f) / 2.5f + 0.6f;
      program.set_vec3("u_modifier", glm::vec3(wave, wave, wave));

      glm::mat4 identity = glm::mat4(1.0f);
      glm::mat4 model =
          glm::rotate(identity, glm::radians(-55.0f * run_time / 1000.f),
                      glm::vec3(1.0f, 0.0f, 0.0f));
      glm::mat4 projection =
          glm::perspective(glm::radians(45.0f), 800.0f / 640.0f, 0.1f, 100.0f);
      program.set_mat4("u_model", model);
      program.set_mat4("u_projection", projection);

      auto view = m_camera->get_view_matrix();
      program.set_mat4("u_view", view);
    }

    m_renderer->render(mesh, program);
    m_window->process_input(*m_camera, delta_time);
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
