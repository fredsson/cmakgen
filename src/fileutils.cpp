#include "../include/fileutils.h"
#include <optional>
#include <functional>
#include <experimental/filesystem>
#include <iostream>

namespace filesystem = std::experimental::filesystem;

namespace {

  bool containsSourceDirectory(const filesystem::path& path) {
    for (auto& entry: filesystem::directory_iterator(path)) {
      if (filesystem::is_directory(entry) && entry.path().generic_string().find("src") != std::string::npos) {
        return true;
      }
    }
    return false;
  }

  std::vector<std::string> searchForDirectories(const filesystem::path& path, std::function<bool(const filesystem::path&)> filter) {
    std::vector<std::string> projectFolders = {};
    for (auto& entry: filesystem::directory_iterator(path)) {
      if (filesystem::is_directory(entry) && filter(entry)) {
        projectFolders.push_back(entry.path().filename());
      }
    }

    return projectFolders;
  }

  std::vector<std::string> getFiles(const filesystem::path& path, const std::string& extension) {
    std::vector<std::string> files = {};
    for (const auto& entry : filesystem::directory_iterator(path)) {
      if (entry.path().extension().generic_string() == extension) {
        files.push_back(entry.path().filename());
      }
    }
    return files;
  }
}

namespace file_utils {
  std::shared_ptr<std::ofstream> createCmakeFile() {
    const auto file = std::shared_ptr<std::ofstream>(new std::ofstream("CMakeLists.txt"));
    return file->is_open() ? file : nullptr;
  }
  std::shared_ptr<std::ofstream> createCmakeFile(const std::string& subFolderName) {
    const auto file = std::shared_ptr<std::ofstream>(new std::ofstream(subFolderName + "/CMakeLists.txt"));
    return file->is_open() ? file : nullptr;
  }

  std::vector<std::string> getIncludeFiles(const std::string& subFolderName) {
    return getFiles(filesystem::current_path().append(subFolderName + "/include"), ".h");
  }

  std::vector<std::string> getSourceFiles(const std::string& subFolderName) {
    return getFiles(filesystem::current_path().append(subFolderName + "/src"), ".cpp");
  }

  void createDir(const std::string& name) {
    filesystem::create_directory(filesystem::current_path().append(name));
  }

  std::string currentDirName() {
    return filesystem::current_path().filename().generic_string();
  }

  std::vector<std::string> getSubProjectDirectories() {
    const auto currentPath = filesystem::current_path();
    return searchForDirectories(filesystem::current_path(), containsSourceDirectory);
  }
}