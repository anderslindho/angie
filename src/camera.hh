#pragma once

#include <glm/glm.hpp>

enum class Direction { k_forward, k_backward, k_left, k_right, k_up, k_down };

class Camera {
private:
  glm::vec3 m_position;
  glm::vec3 m_direction{0.f, 0.f, -1.f};
  glm::vec3 m_up{0.f, 1.f, 0.f};
  glm::vec3 m_right;

  float m_speed{5.f};
  float m_sensitivity{7.f};

  float m_yaw{-90.f};
  float m_pitch{0.f};
  double m_last_mouse_x{0.f};
  double m_last_mouse_y{0.f};

public:
  Camera(glm::vec3 position) : m_position(position) {}
  Camera(float x, float y, float z) : m_position(glm::vec3(x, y, z)) {}

  glm::mat4 get_view_matrix() const;
  void move(Direction direction, float delta_time);
  void handle_mouse_movement(double x_pos, double y_pos, float delta_time);

private:
  void update_view_matrix();
};
