#pragma once

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

#include "indexbuffer.hh"
#include "texture.hh"
#include "vertexattributes.hh"
#include "vertexbuffer.hh"

class Renderer {
private:
  // const unsigned int m_id;

public:
  void prepare() const;
  void render(const std::unique_ptr<VertexAttributes> &vao,
              const std::unique_ptr<IndexBuffer> &ebo) const;
};

void framebuffer_size_callback(GLFWwindow * /* window */, int width,
                               int height);
void process_input(GLFWwindow *window);
