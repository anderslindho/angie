#pragma once

#include <cstddef>
#include <vector>

#include "vertexbuffer.h"

// should maybe bind different vb & ib combinations
class VertexArray {
private:
  unsigned int m_id;
  unsigned int m_stride;
  std::size_t m_nbr_of_attrs{0};

public:
  VertexArray(unsigned int stride);
  ~VertexArray();
  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;

  void bind() const;
  void destroy();

  void add_attribute(unsigned int index, unsigned int size);
};
