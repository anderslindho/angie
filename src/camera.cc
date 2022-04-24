#include "camera.hh"

glm::vec4 Camera::get_view_matrix() const {
  return glm::lookAt(m_position, m_position + m_direction, m_up);
}
