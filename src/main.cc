#include "application.hh"

#include <cstdlib>
#include <string>

const int k_width = 800;
const int k_height = 640;

int main(int /* argc */, char ** /* argv */) {
  auto app = new Application(k_width, k_height);
  app->run();
  delete app;
  std::exit(EXIT_SUCCESS);
}
