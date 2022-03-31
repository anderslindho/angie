#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

std::string read_file(const std::string &fname) {
  std::ifstream ifs(fname, std::ios::in);
  if (!ifs.is_open()) {
    std::cerr << "Failed to load file " << fname << std::endl;
    return "";
  }

  return std::string((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
}
