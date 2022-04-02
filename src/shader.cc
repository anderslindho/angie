#include "shader.hh"

#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

Shader::Shader(const std::string &vertex, const std::string &fragment)
    : m_id(0), m_vertex_src(vertex), m_fragment_src(fragment) {
  int success;
  char info_log[512];

  const auto vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex);
  const auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment);

  m_id = glCreateProgram();
  glAttachShader(m_id, vertex_shader);
  glAttachShader(m_id, fragment_shader);
  glLinkProgram(m_id);
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_id, 512, nullptr, info_log);
    spdlog::error("Failed to link shader program\n{}", info_log);
  } else {
    spdlog::debug("Created shader program from {} and {}", vertex, fragment);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::set_bool(const std::string &name, const bool val) {
  glUniform1i(get_uniform_location(name), static_cast<int>(val));
}

void Shader::set_int(const std::string &name, const int val) {
  glUniform1i(get_uniform_location(name), val);
}

void Shader::set_float(const std::string &name, const float val) {
  glUniform1f(get_uniform_location(name), val);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &val) {
  glUniform3fv(get_uniform_location(name), 1, &val[0]);
}

void Shader::set_vec3(const std::string &name, const float v0, const float v1,
                      const float v2) {
  glUniform3f(get_uniform_location(name), v0, v1, v2);
}

int Shader::get_uniform_location(const std::string &name) {
  if (m_cache.find(name) != m_cache.end()) {
    return m_cache[name];
  }
  auto location = glGetUniformLocation(m_id, name.c_str());
  m_cache[name] = location;
  return location;
}

unsigned int Shader::compile_shader(unsigned int type,
                                    const std::string &path) const {
  int success;
  char info_log[512];

  const auto shader = glCreateShader(type);
  const std::string src = read_file(path);
  const char *src_c = src.c_str();
  glShaderSource(shader, 1, &src_c, nullptr);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, info_log);
    spdlog::error("Failed to compile shader {}: {}",
                  type == GL_VERTEX_SHADER ? "vertex" : "fragment", info_log);
  }
  return shader;
}

const std::string Shader::read_file(const std::string &fname) const {
  std::ifstream ifs(fname, std::ios::in);
  if (!ifs.is_open()) {
    spdlog::error("Failed to load file {}", fname);
    return "";
  }

  return std::string((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
}
