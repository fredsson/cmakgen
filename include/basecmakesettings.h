#ifndef BASE_CMAKE_SETTINGS_H
#define BASE_CMAKE_SETTINGS_H
#include <string>

class BaseCmakeSettings {
public:
  BaseCmakeSettings(std::string projectBuildType, std::string cmakeVersion, std::string cppVersion);
  const std::string& projectBuildType() const;
  const std::string& cmakeVersion() const;
  const std::string& cppVersion() const;
private:
  std::string projectBuildType_;
  std::string cmakeVersion_;
  std::string cppVersion_;
};

#endif