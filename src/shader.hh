#pragma once

#include <string>
#include <unordered_map>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

class Shader {
private:
  unsigned int m_id;
  std::unordered_map<std::string, int> m_cache;
  const std::string m_vertex_src, m_fragment_src;

public:
  Shader(const std::string &vertex, const std::string &fragment);
  ~Shader() { glDeleteProgram(m_id); }
  unsigned int get_id() const { return m_id; }

  void use() const { glUseProgram(m_id); }

  void set_bool(const std::string &name, const bool val);
  void set_int(const std::string &name, const int val);
  void set_float(const std::string &name, const float val);
  void set_vec3(const std::string &name, const glm::vec3 &val);
  void set_vec3(const std::string &name, const float v0, const float v1,
                const float v2);

private:
  int get_uniform_location(const std::string &name);
  unsigned int compile_shader(unsigned int type, const std::string &path) const;
  const std::string read_file(const std::string &path) const;
};
