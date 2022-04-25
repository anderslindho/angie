#pragma once

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

#include "mesh.hh"
#include "shader.hh"

class Renderer {
private:
  // const unsigned int m_id;

public:
  void prepare() const;
  void render(const std::unique_ptr<Mesh> &mesh, const Shader &shader) const;
};

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height);
void process_input(GLFWwindow *window);
