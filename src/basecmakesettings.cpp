#include "../include/basecmakesettings.h"

BaseCmakeSettings::BaseCmakeSettings(std::string projectBuildType, std::string cmakeVersion, std::string cppVersion)
  : projectBuildType_(projectBuildType), cmakeVersion_(cmakeVersion), cppVersion_(cppVersion) {
}

const std::string& BaseCmakeSettings::projectBuildType() const {
  return projectBuildType_;
}

const std::string& BaseCmakeSettings::cmakeVersion() const {
  return cmakeVersion_;
}

const std::string& BaseCmakeSettings::cppVersion() const {
  return cppVersion_;
}