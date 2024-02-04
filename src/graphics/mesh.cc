#include "mesh.hh"

#include "shader.hh"

Mesh::Mesh(const std::vector<float> &positions,
           const std::vector<unsigned int> &indices)
    : m_tb(nullptr), m_positions(positions), m_indices(indices) {
  m_va = std::make_unique<VertexArray>(6);
  m_vb = std::make_unique<VertexBuffer>(positions);
  m_ib = std::make_unique<IndexBuffer>(indices);
  m_va->bind();
  m_vb->bind();
  m_ib->bind();
  m_va->add_attribute(0, 3); // position
  m_va->add_attribute(1, 3); // normals
}

Mesh::~Mesh() {
  // unbind texture first

  m_va->destroy();
  m_vb->destroy();
  m_ib->destroy();
  // we don't necessarily need to destroy texture when mesh is destroyed
  // m_tb->destroy();
}

void Mesh::draw(const Shader &shader) const {
  m_va->bind();
  // m_tb->bind();
  shader.use();
  gl::glDrawElements(gl::GL_TRIANGLES, m_ib->size(), gl::GL_UNSIGNED_INT, 0);
}

void Mesh::add_texture(const std::string &path) {
  m_tb = std::make_unique<Texture>(path);
  m_tb->bind();
}
