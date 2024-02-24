#include <cstdlib>
#include <string>

#include "application.hh"

int main(int /* argc */, char ** /* argv */) {
  auto app = new Application(1400, 800, "angie");
  app->run();
  delete app;
  std::exit(EXIT_SUCCESS);
}
