#include "../include/cmakeproject.h"

CmakeProject CmakeProject::createBaseProject(
  std::string name,
  std::string projectType,
  BaseCmakeSettings baseSettings,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles,
  std::vector<CmakePackage> packages
) {
  return {
    name,
    std::make_optional(projectType),
    std::make_optional(baseSettings),
    {},
    includeFiles,
    sourceFiles,
    packages
  };
}

CmakeProject CmakeProject::createBaseProjectWithSubProjects(
  std::string name,
  BaseCmakeSettings baseSettings,
  std::vector<CmakeProject> subProjects,
  std::vector<CmakePackage> packages
) {
  return {
    name,
    std::nullopt,
    std::make_optional(baseSettings),
    subProjects,
    {},
    {},
    packages
  };
}

CmakeProject CmakeProject::createSubProject(
  std::string name,
  std::string projectType,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles,
  std::vector<CmakePackage> packages
) {
  return {
    name,
    std::make_optional(projectType),
    std::nullopt,
    {},
    includeFiles,
    sourceFiles,
    packages
  };
}

CmakeProject::CmakeProject(
  std::string name,
  std::optional<std::string> projectType,
  std::optional<BaseCmakeSettings> baseSettings,
  std::vector<CmakeProject> subProjects,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles,
  std::vector<CmakePackage> packages
) : name_(name),
 projectType_(projectType),
 baseSettings_(baseSettings),
 subProjects_(subProjects),
 includeFiles_(includeFiles),
 sourceFiles_(sourceFiles),
 packages_(packages) {
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

const std::vector<std::string>& CmakeProject::includeFiles() const {
  return includeFiles_;
}

const std::vector<std::string>& CmakeProject::sourceFiles() const {
  return sourceFiles_;
}

const std::vector<CmakeProject>& CmakeProject::subProjects() const {
  return subProjects_;
}

std::vector<CmakeProject>& CmakeProject::subProjects() {
  return subProjects_;
}

const std::vector<CmakePackage> CmakeProject::packages() const {
  return packages_;
}

void CmakeProject::setIncludeFiles(std::vector<std::string> files) {
  includeFiles_ = files;
}

void CmakeProject::setSourceFiles(std::vector<std::string> files) {
  sourceFiles_ = files;
}