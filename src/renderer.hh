#pragma once

#include <glbinding/gl/gl.h>

class VertexBuffer;
class IndexBuffer;

class Renderer {
private:
  // const unsigned int m_id;
  // temporary members
  unsigned int m_VAO, m_TBO;
  std::unique_ptr<VertexBuffer> m_vbo;
  std::unique_ptr<IndexBuffer> m_ebo;

public:
  Renderer();  // temporary ctor
  ~Renderer(); // temporary dtor

  void prepare() const;
  void render() const;
};
