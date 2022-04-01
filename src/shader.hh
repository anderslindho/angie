#pragma once

#include <string>

class Shader {
private:
  unsigned int m_id;

public:
  Shader(const std::string &vertex, const std::string &fragment);
  ~Shader();

  unsigned int get_id() const { return m_id; }
  void use();
  void set_vec3(const std::string &name, const float x, const float y,
                const float z) const;
};
