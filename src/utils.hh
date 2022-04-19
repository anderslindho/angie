#pragma once

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height) {
  gl::glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
