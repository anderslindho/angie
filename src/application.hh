#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "graphics/renderer.hh"

class Window;
class Camera;

struct Application {
private:
  std::string m_title;
  std::chrono::time_point<std::chrono::system_clock> m_start_time;

  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<Window> m_window;
  std::unique_ptr<Renderer> m_renderer;

public:
  Application(const unsigned int width, const unsigned int height,
              const std::string &title);
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;
  Application(Application &&) = delete;
  Application &operator=(Application &&) = delete;
  ~Application();

  void run() const;
};
