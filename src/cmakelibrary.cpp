#include "../include/cmakelibrary.h"

CmakeLibrary::CmakeLibrary(std::string name, bool internal, std::vector<std::string> options)
  : name_(name), internal_(internal), options_(options) {
}

const std::string& CmakeLibrary::name() const {
  return name_;
}

bool CmakeLibrary::internal() const {
  return internal_;
}

std::optional<std::string> CmakeLibrary::location() const {
  return location_;
}

const std::vector<std::string>& CmakeLibrary::options() const {
  return options_;
}

void CmakeLibrary::setLocation(std::string location) {
  location_ = location;
}

