#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "camera/camera.hh"
#include "graphics/renderer.hh"

class Window;
class CameraController;

struct Application {
private:
  std::string m_title;

  std::unique_ptr<Window> m_window;
  std::unique_ptr<Renderer> m_renderer;
  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<CameraController> m_camera_controller;

public:
  Application(const unsigned int width, const unsigned int height,
              const std::string &title);
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;
  Application(Application &&) = delete;
  Application &operator=(Application &&) = delete;
  ~Application();

  void run() const;

private:
  std::chrono::time_point<std::chrono::system_clock> m_start_time;
  void handle_resize(int width, int height) {
    m_camera->set_aspect_ratio(static_cast<float>(width) /
                               static_cast<float>(height));
  };
};
