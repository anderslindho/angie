#pragma once

#include <cstddef>

class VertexAttributes {
private:
  unsigned int id;
  unsigned int m_stride;
  std::size_t m_nbr_of_attrs{0};

public:
  VertexAttributes(unsigned int stride);
  ~VertexAttributes();
  VertexAttributes(const VertexAttributes &) = delete;
  VertexAttributes &operator=(const VertexAttributes &) = delete;

  void bind() const;
  void add_attribute(unsigned int index, unsigned int size);
};
