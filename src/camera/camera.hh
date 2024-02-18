#pragma once

#include <glm/glm.hpp>

enum class Direction { k_forward, k_backward, k_left, k_right, k_up, k_down };

class Camera {
private:
  glm::vec3 m_position;
  glm::vec3 m_direction{0.f, 0.f, -1.f};

  float m_field_of_view_radians{45.f};
  float m_aspect_ratio{800.f / 640.f};
  float m_near_clip_plane{.1f};
  float m_far_clip_plane{100.f};

  float m_movement_speed{5.f};
  float m_mouse_sensitivity{9.f};

public:
  Camera(glm::vec3 position = glm::vec3{0.f, 0.f, 0.f})
      : m_position(position) {}
  Camera(float x, float y, float z) : m_position(glm::vec3(x, y, z)) {}

  glm::mat4 get_view_matrix() const;
  void set_aspect_ratio(float aspect_ratio);
  glm::mat4 get_projection_matrix() const;

  void move(Direction direction, float delta_time);
  void handle_mouse_movement(double x_pos, double y_pos, float delta_time);

private:
  glm::vec3 m_up{0.f, 1.f, 0.f};
  glm::vec3 m_right;
  glm::vec3 m_world_up{0.f, 1.f, 0.f};

  float m_yaw{-90.f};
  float m_pitch{0.f};

  bool first_mouse{true};
  double m_last_mouse_x;
  double m_last_mouse_y;

  void update_view_matrix();
};
