#include "application.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <glbinding/glbinding.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "camera/cameracontroller.h"
#include "graphics/shader.h"
#include "window.h"

Application::Application(const unsigned int width, const unsigned int height,
                         const std::string &title)
    : m_title(title), m_window(std::make_unique<Window>(width, height, title)),
      m_renderer(std::make_unique<Renderer>()),
      m_camera(std::make_unique<Camera>(.5f, 1.f, 5.f)),
      m_camera_controller(
          std::make_unique<CameraController>(*m_camera, *m_window)) {

  m_window->set_resize_callback(
      [this](int width, int height) { handle_resize(width, height); });

  m_camera->set_aspect_ratio(static_cast<float>(width) /
                             static_cast<float>(height));
  m_start_time = glfwGetTime();
}

void Application::run() const {
  struct Model {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::string texture;
  };

  // These coordinates and indices are shamelessly stolen
  struct Model cube {
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Vertex 0
     0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Vertex 1
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // Vertex 2
     -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Vertex 3

     // Back face
     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Vertex 4
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // Vertex 5
     0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   // Vertex 6
     -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // Vertex 7

     // Right face
     0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // Vertex 8
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Vertex 9
     0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // Vertex 10
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // Vertex 11

     // Left face
     -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  // Vertex 12
     -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Vertex 13
     -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // Vertex 14
     -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   // Vertex 15

     // Top face
     -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // Vertex 16
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // Vertex 17
     0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Vertex 18
     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 19

     // Bottom face
     -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // Vertex 20
     0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  // Vertex 21
     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Vertex 22
     -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},
        {
            0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
            8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
        },
    {
      "container.jpg"
    }
  };

  auto prev_time = glfwGetTime();

  auto box = std::make_unique<Mesh>(cube.vertices, cube.indices);
  auto box_pos = glm::vec3{0.f, 0.f, 0.f};
  Shader colour_prog("colours.vert", "colours.frag");

  auto light = std::make_unique<Mesh>(cube.vertices, cube.indices);
  auto circling_radius = 2.f;
  float angular_velocity = 1.f;
  Shader light_prog("light.vert", "light.frag");

  while (m_window->should_stay_open()) {
    auto time = glfwGetTime();
    const double delta_time = time - prev_time;

    m_renderer->prepare();
    auto projection = m_camera->get_projection_matrix();
    auto view = m_camera->get_view_matrix();

    const double run_time = glfwGetTime() - m_start_time;
    float angle = angular_velocity * run_time;
    float x = box_pos.x + circling_radius * cos(angle);
    float y = box_pos.y + .7f;
    float z = box_pos.z + circling_radius * sin(angle);
    glm::vec3 light_pos(x, y, z);

    {
      // box
      colour_prog.use();

      colour_prog.set_vec3("u_object_colour", 1.f, .5f, .31f);
      colour_prog.set_vec3("u_light_colour", 1.f, 1.f, 1.f);
      colour_prog.set_vec3("u_light_position", light_pos);

      colour_prog.set_mat4("u_projection", projection);
      colour_prog.set_mat4("u_view", view);

      auto model = glm::mat4{1.f};
      model = glm::translate(model, box_pos);
      colour_prog.set_mat4("u_model", model);

      m_renderer->render(box, colour_prog);
    }

    {
      // light
      light_prog.use();

      light_prog.set_mat4("u_projection", projection);
      light_prog.set_mat4("u_view", view);

      auto model = glm::mat4(1.f);
      model = glm::translate(model, light_pos);
      model = glm::scale(model, glm::vec3(.2f));

      light_prog.set_mat4("u_model", model);

      m_renderer->render(light, light_prog);
    }

    m_window->update();
    m_camera_controller->handle_input(delta_time);

    prev_time = time;
  }
}

Application::~Application() {
  const auto run_time = glfwGetTime();
  spdlog::info("Program ran for {} seconds.", std::round(run_time));
}
