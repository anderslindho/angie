#pragma once

#include <string>

class Texture {
private:
  unsigned int m_id;

public:
  Texture(const std::string &image);
  ~Texture();
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
};
