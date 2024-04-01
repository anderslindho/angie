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
#include "graphics/texture.h"
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
    std::string specular_map;
  };

  Model cube{{
                 // position          // normal            // texture
                 -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
                 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
                 -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

                 -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
                 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
                 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
                 -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,

                 0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                 0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                 0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

                 -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,
                 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
                 -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
                 -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,

                 -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                 0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                 -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

                 -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
                 0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
                 0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
                 -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
             },
             {
                 0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
                 8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
                 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
             },
             {"container2.png"},
             {"container2_specular.png"}};

  auto prev_time = glfwGetTime();

  auto box = std::make_unique<Mesh>(cube.vertices, cube.indices);
  box->add_texture(cube.texture);
  box->add_specular_map(cube.specular_map);
  auto box_pos = glm::vec3{0.f, 0.f, 0.f};

  Shader colour_prog("colours.vert", "colours.frag");
  colour_prog.use();
  colour_prog.set_vec3("u_material.ambient", 1.f, .5f, .31f);
  colour_prog.set_float("u_material.shininess", 32.f);
  colour_prog.set_int("u_material.diffuse", 0);
  colour_prog.set_int("u_material.specular", 1);

  auto light = std::make_unique<Mesh>(cube.vertices, cube.indices);
  auto circling_radius = 2.f;
  float angular_velocity = 1.f;
  Shader light_prog("light.vert", "light.frag");
  auto light_colour = glm::vec3{1.f, 1.f, .8f};
  light_prog.use();
  light_prog.set_vec3("u_light_colour", light_colour);

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

      colour_prog.set_vec3("u_light.position", light_pos);
      colour_prog.set_vec3("u_light.ambient",
                           light_colour * glm::vec3{.2f, .2f, .2f});
      colour_prog.set_vec3("u_light.diffuse",
                           light_colour * glm::vec3{.5f, .5f, .5f});
      colour_prog.set_vec3("u_light.specular", 1.f, 1.f, 1.f);

      colour_prog.set_vec3("u_view_position", m_camera->get_position());

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
