#pragma once

#include <string>

class Texture {
private:
  unsigned int m_id;
  std::string m_fname;
  int m_width, m_height, m_channel_count;
  std::string m_texture_dir{"res/textures/"};

public:
  Texture(const std::string &fname, int unit);
  ~Texture();
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;

  void bind(int unit) const;
  void unbind() const;
  void destroy();
};
