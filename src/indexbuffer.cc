#include "indexbuffer.hh"

#include <glbinding/gl/gl.h>

IndexBuffer::IndexBuffer(const std::vector<unsigned int> &indices)
    : m_index_count(indices.size()) {
  gl::glGenBuffers(1, &m_id);
  gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_id);
  gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER,
                   indices.size() * sizeof(unsigned int), &indices.front(),
                   gl::GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { gl::glDeleteBuffers(1, &m_id); }

void IndexBuffer::bind() const {
  gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, m_id);
}

unsigned int IndexBuffer::size() const { return m_index_count; };
