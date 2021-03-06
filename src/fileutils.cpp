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

  std::vector<std::string> getFiles(const filesystem::path& path, const std::string& subfolderName, const std::string& fileFolder, const std::string& extension) {
    try {
      std::vector<std::string> files = {};
      for (const auto& entry : filesystem::directory_iterator(path)) {
        if (entry.status().type() == filesystem::file_type::directory) {
          std::vector<std::string> subFiles = getFiles(entry.path(), entry.path().filename().generic_string() + "/", fileFolder, extension);
          files.insert(files.end(), subFiles.begin(), subFiles.end());
        }
        if (entry.path().extension().generic_string() == extension) {
          files.push_back(fileFolder + subfolderName + entry.path().filename().generic_string());
        }
      }
      return files;
    } catch (filesystem::filesystem_error e) {
      return {};
    }
  }
}

namespace file_utils {
  std::vector<std::string> getIncludeFiles(const std::string& subfolderName) {
    return getFiles(filesystem::current_path().append(subfolderName + "/include"), "", "include/", ".h");
  }

  std::vector<std::string> getSourceFiles(const std::string& subfolderName) {
    return getFiles(filesystem::current_path().append(subfolderName + "/src"), "", "src/", ".cpp");
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