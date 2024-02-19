#pragma once

#include "../window.hh"
#include "camera.hh"

class CameraController {
private:
  Camera &m_camera;
  Window &m_window;

  float m_movement_speed{5.f};
  float m_mouse_sensitivity{9.f};

public:
  CameraController(Camera &camera, Window &window);

  void handle_input(float delta_time);
};
