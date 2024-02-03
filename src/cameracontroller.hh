#pragma once

#include "camera.hh"
#include "window.hh"

class CameraController {
private:
  Camera &m_camera;
  Window &m_window;

public:
  CameraController(Camera &camera, Window &window);

  void handle_input(float delta_time);
};
