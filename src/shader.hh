#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Shader {
private:
  unsigned int m_id;
  const std::string m_vertex_src, m_fragment_src;

public:
  Shader(const std::string &vertex, const std::string &fragment);
  ~Shader() { glDeleteProgram(m_id); }
  unsigned int get_id() const { return m_id; }

  void use() const { glUseProgram(m_id); }

  void set_vec3(const std::string &name, const float x, const float y,
                const float z) const;

private:
  unsigned int compile_shader(const std::string &path, GLenum type) const;
  const std::string read_file(const std::string &path) const;
};
