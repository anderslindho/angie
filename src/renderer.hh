#pragma once

#include <glbinding/gl/gl.h>

class VertexBuffer;

class Renderer {
private:
  // const unsigned int m_id;
  // temporary members
  unsigned int m_VAO, m_TBO, m_EBO;
  std::unique_ptr<VertexBuffer> m_vbo;

public:
  Renderer();  // temporary ctor
  ~Renderer(); // temporary dtor

  void prepare() const;
  void render() const;
};
