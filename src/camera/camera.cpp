#include "camera.h"

#include <spdlog/spdlog.h>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::set_aspect_ratio(float aspect_ratio) {
  m_aspect_ratio = aspect_ratio;
}

glm::mat4 Camera::get_projection_matrix() const {
  return glm::perspective(glm::radians(m_field_of_view_radians), m_aspect_ratio,
                          m_near_clip_plane, m_far_clip_plane);
}

void Camera::move(Direction direction, float magnitude) {
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

void Camera::reorient(double x_pos, double y_pos, float magnitude) {
  if (first_mouse) {
    m_last_mouse_x = x_pos;
    m_last_mouse_y = y_pos;
    first_mouse = false;
  }

  spdlog::debug("x: {}, y: {}", x_pos, y_pos);
  double delta_x = x_pos - m_last_mouse_x;
  double delta_y = y_pos - m_last_mouse_y;

  m_yaw += static_cast<float>(delta_x) * magnitude;
  m_pitch -= static_cast<float>(delta_y) * magnitude;

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
