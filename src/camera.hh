#pragma once

#include <glm/gtc/matrix_transform.hpp>

enum class Direction { k_forward, k_backward, k_left, k_right };

class Camera {
private:
  glm::vec4 m_position;

public:
  Camera();
  ~Camera();

  glm::vec4 get_view_matrix() const;

private:
  bool first_mouse{true};
  void update();
}
