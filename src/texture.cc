#include "texture.hh"

#include <spdlog/spdlog.h>

#include <glbinding/gl/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &fname) : m_fname(fname) {
  buf = stbi_load(fname.c_str(), &m_width, &m_height, &m_channel_count, 0);
  if (!buf)
    spdlog::error("Failed to load image {}", fname);

  gl::glGenTextures(1, &m_id);
  gl::glBindTexture(gl::GL_TEXTURE_2D, m_id);
  gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGB, m_width, m_height, 0,
                   gl::GL_RGB, gl::GL_UNSIGNED_BYTE, buf);
  gl::glGenerateMipmap(gl::GL_TEXTURE_2D);
  stbi_image_free(buf);
}

Texture::~Texture() { gl::glDeleteTextures(1, &m_id); }
