#ifndef CMAKE_FILE_H
#define CMAKE_FILE_H
#include <fstream>
#include <memory>
#include <optional>

#include "cmakeproject.h"

namespace CmakeFile {
  void save(std::shared_ptr<std::ofstream> fileStream, const CmakeProject& project);
  std::optional<CmakeProject> load(std::shared_ptr<std::ifstream> fileStream, bool hasParent);
};

#endif