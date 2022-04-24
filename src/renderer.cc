#include "renderer.hh"

#include <vector>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "indexbuffer.hh"
#include "vertexattributes.hh"
#include "vertexbuffer.hh"

gl::GLenum check_error_(const char *file, int line) {
  gl::GLenum error_code;
  while ((error_code = gl::glGetError()) != gl::GL_NO_ERROR) {
    std::string error;
    switch (error_code) {
    case gl::GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case gl::GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case gl::GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case gl::GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case gl::GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    default:
      error = "UNKNOWN";
      break;
    }
    spdlog::error("GL {} | {}:{}", error, file, line);
  }
  return error_code;
}
#define check_error() check_error_(__FILE__, __LINE__)

Renderer::Renderer() {
  std::vector<float> vertices = {
      // positions        // colors         // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };
  std::vector<unsigned int> indices = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  int image_width, image_height, image_channel_count;
  std::string image = "res/textures/container.jpg";
  void *image_data = stbi_load(image.c_str(), &image_width, &image_height,
                               &image_channel_count, 0);
  if (!image_data)
    spdlog::error("Failed to load image {}", image);

  m_vbo = std::make_unique<VertexBuffer>(vertices);
  m_ebo = std::make_unique<IndexBuffer>(indices);
  m_vao = std::make_unique<VertexAttributes>(8);

  gl::glGenTextures(1, &m_TBO);

  m_vao->bind();
  gl::glBindTexture(gl::GL_TEXTURE_2D, m_TBO);
  gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGB, image_width, image_height,
                   0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, image_data);
  gl::glGenerateMipmap(gl::GL_TEXTURE_2D);

  stbi_image_free(image_data);

  m_vao->add_attribute(0, 3); // position
  m_vao->add_attribute(1, 3); // colour
  m_vao->add_attribute(2, 2); // texture
}

Renderer::~Renderer() { gl::glDeleteTextures(1, &m_TBO); }

void Renderer::prepare() const {
  gl::glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
  gl::glClear(gl::GL_COLOR_BUFFER_BIT);
}

void Renderer::render() const {
  m_vao->bind();
  m_ebo->bind();
  gl::glDrawElements(gl::GL_TRIANGLES, m_ebo->size(), gl::GL_UNSIGNED_INT, 0);
  check_error(); // simplistic usage; basically just wipes the error stack but
                 // does, however, occasionally catch stuff
}
