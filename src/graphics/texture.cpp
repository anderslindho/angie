#include "texture.h"

#include <spdlog/spdlog.h>

#include <glbinding/gl/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &fname, int unit = 0)
    : m_fname(fname), m_width(0), m_height(0), m_channel_count(0) {
  std::string path{m_texture_dir + fname};
  auto buf = stbi_load(path.c_str(), &m_width, &m_height, &m_channel_count, 0);
  if (!buf)
    spdlog::error("Failed to load image {}", path);

  gl::GLenum format{0};
  if (m_channel_count == 1)
    format = gl::GL_RED;
  else if (m_channel_count == 3)
    format = gl::GL_RGB;
  else if (m_channel_count == 4)
    format = gl::GL_RGBA;
  else
    spdlog::error("Not recognised channel count {}", m_channel_count);

  gl::glGenTextures(1, &m_id);
  bind(unit);
  gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format,
                   gl::GL_UNSIGNED_BYTE, buf);
  gl::glGenerateMipmap(gl::GL_TEXTURE_2D);

  gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_S, gl::GL_REPEAT);
  gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_WRAP_T, gl::GL_REPEAT);
  gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER,
                      gl::GL_LINEAR_MIPMAP_LINEAR);
  gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER,
                      gl::GL_LINEAR);

  stbi_image_free(buf);
}

Texture::~Texture() { destroy(); }

void Texture::bind(int unit = 0) const {
  gl::glActiveTexture(gl::GL_TEXTURE0 + unit);
  gl::glBindTexture(gl::GL_TEXTURE_2D, m_id);
}

void Texture::unbind() const { gl::glBindTexture(gl::GL_TEXTURE_2D, 0); }

void Texture::destroy() {
  if (m_id != 0) {
    gl::glDeleteTextures(1, &m_id);
    m_id = 0;
  }
}
