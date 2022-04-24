#pragma once

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
  void render(const VertexAttributes *const vao,
              const IndexBuffer *const ebo) const;
};
