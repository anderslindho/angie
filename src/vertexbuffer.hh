#pragma once

#include <vector>

class VertexBuffer {
private:
  unsigned int id;
  unsigned int vertex_count;

public:
  VertexBuffer(const std::vector<float> &vertices);
  ~VertexBuffer();
  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer &operator=(const VertexBuffer &) = delete;

  void bind() const;
};
