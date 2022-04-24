#pragma once

#include <vector>

class VertexBuffer {
private:
  unsigned int m_id;

public:
  VertexBuffer(const std::vector<float> &vertices);
  ~VertexBuffer();
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer &operator=(const VertexBuffer &) = delete;

  void bind() const;
};
