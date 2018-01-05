#include "../include/cmakelibrary.h"

CmakeLibrary::CmakeLibrary(std::string name, std::vector<std::string> preOptions, std::vector<std::string> options, std::vector<std::string> modules, LibraryType type)
  : name_(name), preOptions_(preOptions), options_(options), modules_(modules), type_(type) {
}

const std::string& CmakeLibrary::name() const {
  return name_;
}

const std::vector<std::string>& CmakeLibrary::preOptions() const {
  return preOptions_;
}

const std::vector<std::string>& CmakeLibrary::options() const {
  return options_;
}

const std::vector<std::string>& CmakeLibrary::modules() const {
  return modules_;
}

const CmakeLibrary::LibraryType CmakeLibrary::type() const {
  return type_;
}
