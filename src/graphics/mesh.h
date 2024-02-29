#pragma once

#include <vector>

#include "indexbuffer.h"
#include "texture.h"
#include "vertexarray.h"

class Shader;

class Mesh {
private:
  std::unique_ptr<VertexArray> m_va;
  std::unique_ptr<VertexBuffer> m_vb;
  std::unique_ptr<IndexBuffer> m_ib;
  std::unique_ptr<Texture> m_tb;

  std::vector<float> m_positions;
  std::vector<unsigned int> m_indices;

public:
  Mesh(const std::vector<float> &positions,
       const std::vector<unsigned int> &indices);
  ~Mesh();
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  void draw(const Shader &shader) const;
  void add_texture(const std::string &path);
};
