#ifndef CMAKE_FILE_H
#define CMAKE_FILE_H
#include <fstream>
#include <memory>
#include "cmakeproject.h"

namespace CmakeFile {
  void save(std::shared_ptr<std::ofstream> fileStream, const CmakeProject& project);
  CmakeProject load(std::shared_ptr<std::ifstream> fileStream);
};

#endif