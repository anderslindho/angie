#include "camera.hh"

#include <spdlog/spdlog.h>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::move(Direction direction, float delta_time) {
  auto magnitude = m_movement_speed * delta_time;
  switch (direction) {
  case Direction::k_forward:
    m_position += m_direction * magnitude;
    break;
  case Direction::k_backward:
    m_position -= m_direction * magnitude;
    break;
  case Direction::k_left:
    m_position -= glm::normalize(glm::cross(m_direction, m_up)) * magnitude;
    break;
  case Direction::k_right:
    m_position += glm::normalize(glm::cross(m_direction, m_up)) * magnitude;
    break;
  case Direction::k_up:
    m_position += m_up * magnitude;
    break;
  case Direction::k_down:
    m_position -= m_up * magnitude;
    break;
  }
}

void Camera::handle_mouse_movement(double x_pos, double y_pos,
                                   float delta_time) {
  if (first_mouse) {
    m_last_mouse_x = x_pos;
    m_last_mouse_y = y_pos;
    first_mouse = false;
  }

  spdlog::debug("x: {}, y: {}", x_pos, y_pos);
  double delta_x = x_pos - m_last_mouse_x;
  double delta_y = y_pos - m_last_mouse_y;

  m_yaw += static_cast<float>(delta_x) * m_mouse_sensitivity * delta_time;
  m_pitch -= static_cast<float>(delta_y) * m_mouse_sensitivity * delta_time;

  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f)
    m_pitch = -89.0f;

  update_view_matrix();

  m_last_mouse_x = x_pos;
  m_last_mouse_y = y_pos;
}

void Camera::update_view_matrix() {
  glm::vec3 new_direction;
  new_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  new_direction.y = sin(glm::radians(m_pitch));
  new_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_direction = glm::normalize(new_direction);

  m_right = glm::normalize(glm::cross(m_direction, m_world_up));
  m_up = glm::normalize(glm::cross(m_right, m_direction));
}
