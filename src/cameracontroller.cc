#include "cameracontroller.hh"

#include <glm/glm.hpp>

CameraController::CameraController(Camera &camera, Window &window)
    : m_camera(camera), m_window(window) {}

void CameraController::handle_input(float delta_time) {
  if (m_window.is_key_pressed(GLFW_KEY_W))
    m_camera.move(Direction::k_forward, delta_time);
  if (m_window.is_key_pressed(GLFW_KEY_S))
    m_camera.move(Direction::k_backward, delta_time);
  if (m_window.is_key_pressed(GLFW_KEY_A))
    m_camera.move(Direction::k_left, delta_time);
  if (m_window.is_key_pressed(GLFW_KEY_D))
    m_camera.move(Direction::k_right, delta_time);
  if (m_window.is_key_pressed(GLFW_KEY_SPACE))
    m_camera.move(Direction::k_up, delta_time);
  if (m_window.is_key_pressed(GLFW_KEY_LEFT_CONTROL))
    m_camera.move(Direction::k_down, delta_time);

  if (m_window.is_key_pressed(GLFW_KEY_ESCAPE))
    m_window.close();

  double x_pos, y_pos;
  m_window.get_cursor_position(x_pos, y_pos);
  m_camera.handle_mouse_movement(x_pos, y_pos, delta_time);
}
