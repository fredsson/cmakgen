#ifndef CMAKE_LIBRARY_H
#define CMAKE_LIBRARY_H
#include <vector>
#include <string>
#include <optional>

class CmakeLibrary {
public:
  enum LibraryType {PACKAGE};
  CmakeLibrary(std::string name, std::vector<std::string> preOptions, std::vector<std::string> options, std::vector<std::string> modules, LibraryType type);

  const std::string& name() const;
  const std::vector<std::string>& preOptions() const;
  const std::vector<std::string>& options() const;
  const std::vector<std::string>& modules() const;
  const LibraryType type() const;

private:
  std::string name_;
  std::vector<std::string> preOptions_;
  std::vector<std::string> options_;
  std::vector<std::string> modules_;
  LibraryType type_;
};

#endif