#pragma once

#include <string>

class Texture {
private:
  unsigned int m_id;
  std::string m_path;
  int m_width, m_height, m_channel_count;

public:
  Texture(const std::string &path);
  ~Texture();
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;

  void bind() const;
  void destroy();
};
