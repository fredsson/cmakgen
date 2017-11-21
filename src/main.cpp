#include <iostream>
#include "../include/cmakeproject.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "no arguments provided\n";
  }

  const auto project = CmakeProject::createBaseProject("test", BaseCmakeSettings{"Debug", "2.8.9", "c++11"}, {});

  const auto command = std::string(argv[1]);
  if (command == "--watch" || command == "-w") {
    std::cout << "received watch command\n";
  } else if (command == "--gen" || command == "-g") {
    std::cout << "received gen command\n";
  } else if (command == "--dep" || command == "-d") {
    std::cout << "received dep command\n";
  } else {
    // print help
  }

  return 0;
}