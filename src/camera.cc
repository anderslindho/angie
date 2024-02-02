#include "camera.hh"

glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(m_position, m_position + m_direction, m_up);
}

void Camera::move(Direction direction) {
  switch (direction) {
  case Direction::k_forward:
    m_position += m_direction * speed;
    break;
  case Direction::k_backward:
    m_position -= m_direction * speed;
    break;
  case Direction::k_left:
    break;
  case Direction::k_right:
    break;
  }
}
