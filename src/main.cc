#include "application.hh"

#include <cstdlib>
#include <string>

const int k_width = 800;
const int k_height = 640;

int main(int argc, char **argv) {
  int width, height;
  if (argc == 3) {
    width = std::stoi(argv[1]);
    height = std::stoi(argv[2]);
  } else {
    width = k_width;
    height = k_height;
  }
  auto app = new Application(width, height);

  app->run();

  delete app;
  std::exit(EXIT_SUCCESS);
}
