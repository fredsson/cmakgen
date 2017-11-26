#include "../include/cmakeproject.h"

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

CmakeProject CmakeProject::createBaseProject(
  std::string name,
  std::string projectType,
  BaseCmakeSettings baseSettings,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles
) {
  return {
    name,
    std::make_optional(projectType),
    std::make_optional(baseSettings),
    {},
    includeFiles,
    sourceFiles
  };
}

CmakeProject CmakeProject::createBaseProjectWithSubProjects(
  std::string name,
  BaseCmakeSettings baseSettings,
  std::vector<CmakeProject> subProjects
) {
  return {
    name,
    std::nullopt,
    std::make_optional(baseSettings),
    subProjects,
    {},
    {}
  };
}

CmakeProject CmakeProject::createSubProject(
  std::string name,
  std::string projectType,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles
) {
  return {
    name,
    std::make_optional(projectType),
    std::nullopt,
    {},
    includeFiles,
    sourceFiles
  };
}

CmakeProject::CmakeProject(
  std::string name,
  std::optional<std::string> projectType,
  std::optional<BaseCmakeSettings> baseSettings,
  std::vector<CmakeProject> subProjects,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles
) : name_(name),
 projectType_(projectType),
 baseSettings_(baseSettings),
 subProjects_(subProjects),
 includeFiles_(includeFiles),
 sourceFiles_(sourceFiles) {
}

const std::string& CmakeProject::name() const {
  return name_;
}

std::optional<std::string> CmakeProject::projectType() const {
  return projectType_;
}

std::optional<BaseCmakeSettings> CmakeProject::baseSettings() const {
  return baseSettings_;
}
const std::vector<CmakeProject>& CmakeProject::subProjects() const {
  return subProjects_;
}

const std::vector<std::string>& CmakeProject::includeFiles() const {
  return includeFiles_;
}

const std::vector<std::string>& CmakeProject::sourceFiles() const {
  return sourceFiles_;
}