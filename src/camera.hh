#pragma once

#include <glm/gtc/matrix_transform.hpp>

enum class Direction { k_forward, k_backward, k_left, k_right };

class Camera {
private:
  glm::vec3 m_position;
  glm::vec3 m_direction{0.f, 0.f, -1.f};
  glm::vec3 m_up{0.f, 1.f, 0.f};

  float speed{.1f};
  // float sensitivity{1.f};

public:
  Camera(glm::vec3 position) : m_position(position) {}
  Camera(float x, float y, float z) : m_position(glm::vec3(x, y, z)) {}

  glm::mat4 get_view_matrix() const;
  void move(Direction direction);

private:
  // bool first_mouse{true};
};
