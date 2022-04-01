#include "shader.hh"

#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

Shader::Shader(const std::string &vertex, const std::string &fragment)
    : m_vertex_src(vertex), m_fragment_src(fragment) {
  int success;
  char info_log[512];

  const auto vertex_shader = compile_shader(vertex, GL_VERTEX_SHADER);
  const auto fragment_shader = compile_shader(fragment, GL_FRAGMENT_SHADER);

  m_id = glCreateProgram();
  glAttachShader(m_id, vertex_shader);
  glAttachShader(m_id, fragment_shader);
  glLinkProgram(m_id);
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_id, 512, nullptr, info_log);
    spdlog::error("Failed to link shader program\n{}", info_log);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::set_vec3(const std::string &name, const float x, const float y,
                      const float z) const {
  const auto location = glGetUniformLocation(m_id, name.c_str());
  glUniform3f(location, x, y, z);
}

unsigned int Shader::compile_shader(const std::string &path,
                                    GLenum type) const {
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
    spdlog::error("Failed to compile shader {}: {}", type, info_log);
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
