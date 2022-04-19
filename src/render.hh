#pragma once

#include <glbinding/gl/gl.h>

class Renderer {
private:
  // const unsigned int m_id;
  unsigned int m_VBO, m_VAO, m_TBO, m_EBO; // temporary members
public:
  Renderer();
  ~Renderer();

  void prepare() const;
  void render() const;
};
