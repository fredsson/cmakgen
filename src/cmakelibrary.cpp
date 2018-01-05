#include "../include/cmakelibrary.h"

CmakeLibrary::CmakeLibrary(std::string name, std::string version, std::vector<std::string> modules, CmakeLibrary::LibraryType type)
  : name_(name), version_(version), modules_(modules), type_(type) {

}

const std::string& CmakeLibrary::name() const {
  return name_;
}

const std::string& CmakeLibrary::version() const {
  return version_;
}

const std::vector<std::string>& CmakeLibrary::modules() const {
  return modules_;
}

const CmakeLibrary::LibraryType CmakeLibrary::type() const {
  return type_;
}
