#pragma once

#include <glm/glm.hpp>

enum class Direction { k_forward, k_backward, k_left, k_right, k_up, k_down };

class Camera {
private:
  glm::vec3 m_position;
  glm::vec3 m_direction;
  glm::vec3 m_up;

  float m_field_of_view_radians{45.f};
  float m_aspect_ratio{4.f / 3.f};
  float m_near_clip_plane{.1f};
  float m_far_clip_plane{100.f};

public:
  Camera(glm::vec3 position = glm::vec3{0.f, 0.f, 0.f})
      : m_position(position), m_direction(0.f, 0.f, -1.f), m_up(0.f, 1.f, 0.f) {
    m_world_up = m_up;
    update_view_matrix();
  }
  Camera(float x, float y, float z) : Camera(glm::vec3(x, y, z)) {}

  glm::mat4 get_view_matrix() const;
  glm::mat4 get_projection_matrix() const;

  void move(Direction direction, float magnitude);
  void reorient(double x_pos, double y_pos, float magnitude);

  void set_aspect_ratio(float aspect_ratio);

private:
  glm::vec3 m_right;
  glm::vec3 m_world_up;

  float m_yaw{-90.f};
  float m_pitch{0.f};

  bool first_mouse{true};
  double m_last_mouse_x;
  double m_last_mouse_y;

  void update_view_matrix();
};
