#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include <string>
#include <vector>

namespace file_utils {
  std::vector<std::string> getIncludeFiles(const std::string& subFolderName = "");
  std::vector<std::string> getSourceFiles(const std::string& subFolderName = "");
  std::vector<std::string> getSubProjectFolders();
}

#endif