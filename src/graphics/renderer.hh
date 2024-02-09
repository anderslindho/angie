#pragma once

#include <glbinding/gl/gl.h>

#include "mesh.hh"
#include "shader.hh"

class Renderer {
public:
  void prepare() const;
  void render(const std::unique_ptr<Mesh> &mesh, const Shader &shader) const;
};
