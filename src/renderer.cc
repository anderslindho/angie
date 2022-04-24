#include "renderer.hh"

#include <vector>

#include <spdlog/spdlog.h>

gl::GLenum check_error_(const char *file, int line) {
  gl::GLenum error_code;
  while ((error_code = gl::glGetError()) != gl::GL_NO_ERROR) {
    std::string error;
    switch (error_code) {
    case gl::GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case gl::GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case gl::GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case gl::GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case gl::GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    default:
      error = "UNKNOWN";
      break;
    }
    spdlog::error("GL {} | {}:{}", error, file, line);
  }
  return error_code;
}
#define check_error() check_error_(__FILE__, __LINE__)

void Renderer::prepare() const {
  gl::glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
  gl::glClear(gl::GL_COLOR_BUFFER_BIT);
}

void Renderer::render(const std::unique_ptr<VertexAttributes> &vao,
                      const std::unique_ptr<IndexBuffer> &ebo) const {
  vao->bind();
  ebo->bind();
  gl::glDrawElements(gl::GL_TRIANGLES, ebo->size(), gl::GL_UNSIGNED_INT, 0);
  check_error(); // simplistic usage; basically just wipes the error stack but
                 // does, however, occasionally catch stuff
}
