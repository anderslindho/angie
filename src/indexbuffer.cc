#include "indexbuffer.hh"

#include <glbinding/gl/gl.h>

IndexBuffer::IndexBuffer(const std::vector<unsigned int> &indices)
    : index_count(indices.size()) {
  gl::glGenBuffers(1, &id);
  gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, id);
  gl::glBufferData(gl::GL_ELEMENT_ARRAY_BUFFER,
                   indices.size() * sizeof(unsigned int), &indices.front(),
                   gl::GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { gl::glDeleteBuffers(1, &id); }

void IndexBuffer::bind() const {
  gl::glBindBuffer(gl::GL_ELEMENT_ARRAY_BUFFER, id);
}

unsigned int IndexBuffer::size() const { return index_count; };
