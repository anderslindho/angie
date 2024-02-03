#include <glm/gtc/matrix_transform.hpp>

#include "camera.hh"

glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::move(Direction direction, float delta_time) {
  switch (direction) {
  case Direction::k_forward:
    m_position += m_direction * speed * delta_time;
    break;
  case Direction::k_backward:
    m_position -= m_direction * speed * delta_time;
    break;
  case Direction::k_left:
    m_position -=
        glm::normalize(glm::cross(m_direction, m_up)) * speed * delta_time;
    break;
  case Direction::k_right:
    m_position +=
        glm::normalize(glm::cross(m_direction, m_up)) * speed * delta_time;
    break;
  }
}
