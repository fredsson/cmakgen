#ifndef CMAKE_PROJECT_H
#define CMAKE_PROJECT_H
#include <string>
#include <optional>
#include <vector>
#include "basecmakesettings.h"
#include "cmakepackage.h"
#include "cmakelibrary.h"

class CmakeProject {
public:
  static CmakeProject createBaseProject(
    std::string name,
    std::string projectType,
    BaseCmakeSettings baseSettings,
    std::vector<std::string> includeFiles,
    std::vector<std::string> sourceFiles,
    std::vector<CmakePackage> packages,
    std::vector<CmakeLibrary> libraries
  );

  static CmakeProject createBaseProjectWithSubProjects(
    std::string name,
    BaseCmakeSettings baseSettings,
    std::vector<CmakeProject> subProjects,
    std::vector<CmakePackage> packages,
    std::vector<CmakeLibrary> libraries
  );

  static CmakeProject createSubProject(
    std::string name,
    std::string projectType,
    bool includeRootDirectory,
    std::vector<std::string> includeFiles,
    std::vector<std::string> sourceFiles,
    std::vector<CmakePackage> packages,
    std::vector<CmakeLibrary> libraries
  );
  
  const std::string& name() const;
  std::optional<std::string> projectType() const;
  bool includeRootDirectory() const;
  std::optional<BaseCmakeSettings> baseSettings() const;
  const std::vector<std::string>& includeFiles() const;
  const std::vector<std::string>& sourceFiles() const;
  const std::vector<CmakeProject>& subProjects() const;
  std::vector<CmakeProject>& subProjects();
  const std::vector<CmakePackage>& packages() const;
  const std::vector<CmakeLibrary>& libraries() const;

  void setIncludeFiles(std::vector<std::string> files);
  void setSourceFiles(std::vector<std::string> files);
  
private:
  CmakeProject(
    std::string name,
    std::optional<std::string> projectType,
    bool includeRootDirectory,
    std::optional<BaseCmakeSettings> baseSettings,
    std::vector<CmakeProject> subProjects,
    std::vector<std::string> includeFiles,
    std::vector<std::string> sourceFiles,
    std::vector<CmakePackage> packages,
    std::vector<CmakeLibrary> libraries
  );

  std::string name_;
  std::optional<std::string> projectType_;
  bool includeRootDirectory_;
  std::optional<BaseCmakeSettings> baseSettings_;
  std::vector<CmakeProject> subProjects_;
  std::vector<std::string> includeDirectories_;
  std::vector<std::string> includeFiles_;
  std::vector<std::string> sourceFiles_;
  std::vector<CmakePackage> packages_;
  std::vector<CmakeLibrary> libraries_;
};

#endif