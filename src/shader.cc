#include "shader.hh"

#include <fstream>
#include <iostream>

#include <spdlog/spdlog.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

const std::string read_file(const std::string &fname) {
  std::ifstream ifs(fname, std::ios::in);
  if (!ifs.is_open()) {
    spdlog::error("Failed to load file {}", fname);
    return "";
  }

  return std::string((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
}

Shader::Shader(const std::string &vertex, const std::string &fragment) {
  int success;
  char info_log[512];

  const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const std::string vertex_shader_src = read_file(vertex);
  const char *vertex_shader_source_C = vertex_shader_src.c_str();
  glShaderSource(vertex_shader, 1, &vertex_shader_source_C, nullptr);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    spdlog::error("Failed to compile vertex shader", info_log);
  }

  const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const std::string fragment_shader_src = read_file(fragment);
  const char *fragment_shader_src_C = fragment_shader_src.c_str();
  glShaderSource(fragment_shader, 1, &fragment_shader_src_C, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    spdlog::error("Failed to compile fragment shader {}", info_log);
  }

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

Shader::~Shader() { glDeleteProgram(m_id); }

void Shader::use() { glUseProgram(m_id); }

void Shader::set_vec3(const std::string &name, const float x, const float y,
                      const float z) const {
  const auto location = glGetUniformLocation(m_id, name.c_str());
  glUniform3f(location, x, y, z);
}
