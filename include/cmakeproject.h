#ifndef CMAKE_PROJECT_H
#define CMAKE_PROJECT_H
#include <string>
#include <optional>
#include <vector>

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

class CmakeProject {
public:
  static CmakeProject createBaseProject(
    std::string name,
    std::string projectType,
    BaseCmakeSettings baseSettings,
    std::vector<std::string> includeFiles,
    std::vector<std::string> sourceFiles
  );

  static CmakeProject createBaseProjectWithSubProjects(
    std::string name,
    BaseCmakeSettings baseSettings,
    std::vector<CmakeProject> subProjects
  );

  static CmakeProject createSubProject(
    std::string name,
    std::string projectType,
    std::vector<std::string> includeFiles,
    std::vector<std::string> sourceFiles
  );
  

  const std::string& name() const;
  std::optional<std::string> projectType() const;
  std::optional<BaseCmakeSettings> baseSettings() const;
  const std::vector<std::string>& includeFiles() const;
  const std::vector<std::string>& sourceFiles() const;
  const std::vector<CmakeProject>& subProjects() const;
  std::vector<CmakeProject>& subProjects();

  void setIncludeFiles(std::vector<std::string> files);
  void setSourceFiles(std::vector<std::string> files);
  
private:
  CmakeProject(
    std::string name,
    std::optional<std::string> projectType,
    std::optional<BaseCmakeSettings> baseSettings,
    std::vector<CmakeProject> subProjects,
    std::vector<std::string> includeFiles,
    std::vector<std::string> sourceFiles
  );

  std::string name_;
  std::optional<std::string> projectType_;
  std::optional<BaseCmakeSettings> baseSettings_;
  std::vector<CmakeProject> subProjects_;
  std::vector<std::string> includeFiles_;
  std::vector<std::string> sourceFiles_;
};

#endif