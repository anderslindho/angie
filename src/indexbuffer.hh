#pragma once

#include <vector>

class IndexBuffer {
private:
  unsigned int id;
  unsigned int index_count;

public:
  IndexBuffer(const std::vector<unsigned int> &indices);
  ~IndexBuffer();
  IndexBuffer(const IndexBuffer &) = delete;
  IndexBuffer &operator=(const IndexBuffer &) = delete;

  void bind() const;
  unsigned int size() const;
};
