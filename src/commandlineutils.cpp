#include "../include/commandlineutils.h"
#include <cstdlib>

namespace commandline_utils {
  void executeBuildCommand() {
    std::system("cd _build && cmake ../ && make");
  }

}