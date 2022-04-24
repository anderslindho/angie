#include "vertexattributes.hh"

#include <glbinding/gl/gl.h>

VertexAttributes::VertexAttributes(unsigned int stride) : m_stride(stride) {
  gl::glGenVertexArrays(1, &m_id);
}

VertexAttributes::~VertexAttributes() { gl::glDeleteVertexArrays(1, &m_id); }

void VertexAttributes::bind() const { gl::glBindVertexArray(m_id); }

void VertexAttributes::add_attribute(unsigned int index, unsigned int size) {
  bind();
  gl::glVertexAttribPointer(index, size, gl::GL_FLOAT, gl::GL_FALSE,
                            m_stride * sizeof(float), (void *)m_nbr_of_attrs);
  m_nbr_of_attrs += size * sizeof(float);
  gl::glEnableVertexAttribArray(index);
}
