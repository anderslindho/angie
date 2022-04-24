#pragma once

#include <vector>

class IndexBuffer {
private:
  unsigned int m_id;
  unsigned int m_index_count;

public:
  IndexBuffer(const std::vector<unsigned int> &indices);
  ~IndexBuffer();
  IndexBuffer(const IndexBuffer &) = delete;
  IndexBuffer &operator=(const IndexBuffer &) = delete;

  void bind() const;
  unsigned int size() const;
};
