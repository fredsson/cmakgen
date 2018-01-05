#ifndef CMAKE_LIBRARY_H
#define CMAKE_LIBRARY_H
#include <vector>
#include <string>

class CmakeLibrary {
public:
  const enum LibraryType {PACKAGE};
  CmakeLibrary(std::string name, std::string version, std::vector<std::string> modules, CmakeLibrary::LibraryType type);

  const std::string& name() const;
  const std::string& version() const;
  const std::vector<std::string>& modules() const;
  const LibraryType type() const;

private:
  std::string name_;
  std::string version_;
  std::vector<std::string> modules_;
  LibraryType type_;
};

#endif