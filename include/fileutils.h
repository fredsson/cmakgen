#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace file_utils {
  std::shared_ptr<std::ofstream> createCmakeFile();
  std::shared_ptr<std::ofstream> createCmakeFile(const std::string& subFolderName);
  std::vector<std::string> getIncludeFiles(const std::string& subFolderName);
  std::vector<std::string> getSourceFiles(const std::string& subFolderName);
  void createDir(const std::string& name);
  std::string currentDirName();
  std::vector<std::string> getSubProjectDirectories();
}

#endif