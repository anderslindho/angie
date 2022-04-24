#include "vertexbuffer.hh"

#include <glbinding/gl/gl.h>

VertexBuffer::VertexBuffer(const std::vector<float> &vertices)
    : vertex_count(vertices.size()) {
  gl::glGenBuffers(1, &id);
  gl::glBindBuffer(gl::GL_ARRAY_BUFFER, id);
  gl::glBufferData(gl::GL_ARRAY_BUFFER, vertex_count * sizeof(float),
                   &vertices.front(), gl::GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() { gl::glDeleteBuffers(1, &id); }

void VertexBuffer::bind() const { gl::glBindBuffer(gl::GL_ARRAY_BUFFER, id); }
