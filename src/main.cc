#include "application.hh"

#include <cstdlib>
#include <string>

const int k_width = 800;
const int k_height = 640;
const std::string k_title = "angie";

int main(int /* argc */, char ** /* argv */) {
  auto app = new Application(k_width, k_height, k_title);
  app->run();
  delete app;
  std::exit(EXIT_SUCCESS);
}
