#include "vertexarray.hh"

#include <glbinding/gl/gl.h>

VertexArray::VertexArray(unsigned int stride) : m_stride(stride) {
  gl::glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
  gl::glDeleteVertexArrays(1, &m_id);
  for (auto &vb : vbos)
    vb->release();
}

void VertexArray::bind() const { gl::glBindVertexArray(m_id); }

void VertexArray::add_buffer(const std::vector<float> &vertices) {
  vbos.push_back(std::make_unique<VertexBuffer>(vertices));
}

void VertexArray::add_attribute(unsigned int index, unsigned int size) {
  gl::glVertexAttribPointer(index, size, gl::GL_FLOAT, gl::GL_FALSE,
                            m_stride * sizeof(float), (void *)m_nbr_of_attrs);
  m_nbr_of_attrs += size * sizeof(float);
  gl::glEnableVertexAttribArray(index);
}
