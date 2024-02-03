#pragma once

#include <string>
#include <unordered_map>

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

#include <glm/glm.hpp>

class Shader {
private:
  unsigned int m_id;
  std::unordered_map<std::string, int> m_cache;
  const std::string m_vertex_src, m_fragment_src;
  const std::string m_shader_dir{"res/shaders/"};

public:
  Shader(const std::string &vertex, const std::string &fragment);
  ~Shader() { gl::glDeleteProgram(m_id); }

  unsigned int get_id() const { return m_id; }
  void use() const { gl::glUseProgram(m_id); }

  void set_bool(const std::string &name, const bool val);
  void set_int(const std::string &name, const int val);
  void set_float(const std::string &name, const float val);
  void set_vec2(const std::string &name, const glm::vec2 &val);
  void set_vec2(const std::string &name, const float v0, const float v1);
  void set_vec3(const std::string &name, const glm::vec3 &val);
  void set_vec3(const std::string &name, const float v0, const float v1,
                const float v2);
  void set_mat3(const std::string &name, const glm::mat3 &val);
  void set_mat4(const std::string &name, const glm::mat4 &val);

private:
  int get_uniform_location(const std::string &name);
  unsigned int compile_shader(const gl::GLenum type,
                              const std::string &path) const;
  const std::string read_file(const std::string &path) const;
};
