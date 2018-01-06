#include "../include/cmakepackage.h"

CmakePackage::CmakePackage(std::string name, std::vector<std::string> preOptions, std::vector<std::string> options, std::vector<std::string> modules)
  : name_(name), preOptions_(preOptions), options_(options), modules_(modules) {
}

const std::string& CmakePackage::name() const {
  return name_;
}

const std::vector<std::string>& CmakePackage::preOptions() const {
  return preOptions_;
}

const std::vector<std::string>& CmakePackage::options() const {
  return options_;
}

const std::vector<std::string>& CmakePackage::modules() const {
  return modules_;
}