#pragma once

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height) {
  gl::glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

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
#define glCheckError() check_error_(__FILE__, __LINE__)
