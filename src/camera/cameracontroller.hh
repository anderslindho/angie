#pragma once

#include "../window.hh"
#include "camera.hh"

class CameraController {
private:
  Camera &m_camera;
  Window &m_window;

public:
  CameraController(Camera &camera, Window &window);

  void handle_input(float delta_time);
};
