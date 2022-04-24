#pragma once

#include <glbinding/gl/gl.h>

#include "indexbuffer.hh"
#include "texture.hh"
#include "vertexattributes.hh"
#include "vertexbuffer.hh"

class Renderer {
private:
  // const unsigned int m_id;
  // temporary members
  std::unique_ptr<Texture> m_texture;
  std::unique_ptr<VertexBuffer> m_vbo;
  std::unique_ptr<VertexAttributes> m_vao;
  std::unique_ptr<IndexBuffer> m_ebo;

public:
  Renderer(); // temporary ctor

  void prepare() const;
  void render() const;
};
