#include "../include/cmakeproject.h"

CmakeProject CmakeProject::createBaseProject(
  std::string name,
  std::string projectType,
  BaseCmakeSettings baseSettings,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles,
  std::vector<CmakePackage> packages,
  std::vector<CmakeLibrary> libraries
) {
  return {
    name,
    std::make_optional(projectType),
    false,
    std::make_optional(baseSettings),
    {},
    includeFiles,
    sourceFiles,
    packages,
    libraries
  };
}

CmakeProject CmakeProject::createBaseProjectWithSubProjects(
  std::string name,
  BaseCmakeSettings baseSettings,
  std::vector<CmakeProject> subProjects,
  std::vector<CmakePackage> packages,
  std::vector<CmakeLibrary> libraries
) {
  return {
    name,
    std::nullopt,
    false,
    std::make_optional(baseSettings),
    subProjects,
    {},
    {},
    packages,
    libraries
  };
}

CmakeProject CmakeProject::createSubProject(
  std::string name,
  std::string projectType,
  bool includeRootDirectory,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles,
  std::vector<CmakePackage> packages,
  std::vector<CmakeLibrary> libraries
) {
  return {
    name,
    std::make_optional(projectType),
    includeRootDirectory,
    std::nullopt,
    {},
    includeFiles,
    sourceFiles,
    packages,
    libraries
  };
}

CmakeProject::CmakeProject(
  std::string name,
  std::optional<std::string> projectType,
  bool includeRootDirectory,
  std::optional<BaseCmakeSettings> baseSettings,
  std::vector<CmakeProject> subProjects,
  std::vector<std::string> includeFiles,
  std::vector<std::string> sourceFiles,
  std::vector<CmakePackage> packages,
  std::vector<CmakeLibrary> libraries
) : name_(name),
 projectType_(projectType),
 includeRootDirectory_(includeRootDirectory),
 baseSettings_(baseSettings),
 subProjects_(subProjects),
 includeFiles_(includeFiles),
 sourceFiles_(sourceFiles),
 packages_(packages),
 libraries_(libraries) {
}

const std::string& CmakeProject::name() const {
  return name_;
}

std::optional<std::string> CmakeProject::projectType() const {
  return projectType_;
}

bool CmakeProject::includeRootDirectory() const {
  return includeRootDirectory_;
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

const std::vector<CmakePackage>& CmakeProject::packages() const {
  return packages_;
}

const std::vector<CmakeLibrary>& CmakeProject::libraries() const {
  return libraries_;
}

void CmakeProject::setIncludeFiles(std::vector<std::string> files) {
  includeFiles_ = files;
}

void CmakeProject::setSourceFiles(std::vector<std::string> files) {
  sourceFiles_ = files;
}