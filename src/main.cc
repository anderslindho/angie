#include "application.hh"

#include <cstdlib>

const int k_width = 800;
const int k_height = 640;

int main(int /* argc */, char ** /*argv*/) {
  auto app = Application(k_width, k_height);
  std::exit(EXIT_SUCCESS);
}
