#include "texture.hh"

#include <spdlog/spdlog.h>

#include <glbinding/gl/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &path)
    : m_path(path), m_width(0), m_height(0), m_channel_count(0) {
  auto buf = stbi_load(path.c_str(), &m_width, &m_height, &m_channel_count, 0);
  if (!buf)
    spdlog::error("Failed to load image {}", path);

  gl::glGenTextures(1, &m_id);
  bind();
  gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGB, m_width, m_height, 0,
                   gl::GL_RGB, gl::GL_UNSIGNED_BYTE, buf);
  gl::glGenerateMipmap(gl::GL_TEXTURE_2D);
  stbi_image_free(buf);
}

Texture::~Texture() { destroy(); }

void Texture::bind() const { gl::glBindTexture(gl::GL_TEXTURE_2D, m_id); }

void Texture::destroy() {
  if (m_id)
    gl::glDeleteTextures(1, &m_id);
}
