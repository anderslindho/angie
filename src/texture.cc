#include "texture.hh"

#include <spdlog/spdlog.h>

#include <glbinding/gl/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &image) {
  int image_width, image_height, image_channel_count;
  void *image_data = stbi_load(image.c_str(), &image_width, &image_height,
                               &image_channel_count, 0);
  if (!image_data)
    spdlog::error("Failed to load image {}", image);

  gl::glGenTextures(1, &m_id);
  gl::glBindTexture(gl::GL_TEXTURE_2D, m_id);
  gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGB, image_width, image_height,
                   0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, image_data);
  gl::glGenerateMipmap(gl::GL_TEXTURE_2D);

  stbi_image_free(image_data);
}

Texture::~Texture() { gl::glDeleteTextures(1, &m_id); }
