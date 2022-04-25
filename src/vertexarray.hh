#pragma once

#include <cstddef>
#include <vector>

#include "vertexbuffer.hh"

// should maybe bind different vb & ib combinations
class VertexArray {
private:
  unsigned int m_id;
  unsigned int m_stride;
  std::size_t m_nbr_of_attrs{0};
  std::vector<std::unique_ptr<VertexBuffer>> vbos{};

public:
  VertexArray(unsigned int stride);
  ~VertexArray();
  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;

  void bind() const;

  void add_buffer(const std::vector<float> &vertices);
  void add_attribute(unsigned int index,
                     unsigned int size); // TODO: turn private
};
