#ifndef CMAKEFILE_H
#define CMAKEFILE_H
#include <memory>
#include <string>
#include <vector>

class CmakeFunction;
class CmakeFile {
public:
  CmakeFile(std::string path, std::vector<std::shared_ptr<CmakeFunction>> functions);

  const std::string& path() const;
  const std::vector<std::shared_ptr<CmakeFunction>>& functions() const;

  void updateFiles(const std::vector<std::string>& includeFiles, const std::vector<std::string>& srcFiles);
private:
  void removeIncludeFilesFunction();
  void addIncludeFilesFunction(const std::shared_ptr<CmakeFunction>& srcFilesFunction, const std::vector<std::string>& includeFiles);

  std::string path_;
  std::vector<std::shared_ptr<CmakeFunction>> functions_;
};

#endif