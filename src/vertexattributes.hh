#pragma once

#include <cstddef>

// should probably become a `VertexArray` that binds different vb & ib
// combinations
class VertexAttributes {
private:
  unsigned int m_id;
  unsigned int m_stride;
  std::size_t m_nbr_of_attrs{0};

public:
  VertexAttributes(unsigned int stride);
  ~VertexAttributes();
  VertexAttributes(const VertexAttributes &) = delete;
  VertexAttributes &operator=(const VertexAttributes &) = delete;

  void bind() const;
  // maybe also move ownership ?
  // void add_buffer(const std::unique_ptr<VertexBuffer> &vb, const
  // std::unique_ptr<IndexBuffer> &ib);

  void add_attribute(unsigned int index,
                     unsigned int size); // TODO: turn private
};
