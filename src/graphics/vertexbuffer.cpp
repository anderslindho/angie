#include "vertexbuffer.h"

#include <glbinding/gl/gl.h>

VertexBuffer::VertexBuffer(const std::vector<float> &vertices) {
  gl::glGenBuffers(1, &m_id);
  bind();
  gl::glBufferData(gl::GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                   &vertices.front(), gl::GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() { destroy(); }

void VertexBuffer::bind() const { gl::glBindBuffer(gl::GL_ARRAY_BUFFER, m_id); }

void VertexBuffer::destroy() const {
  if (m_id)
    gl::glDeleteBuffers(1, &m_id);
}
