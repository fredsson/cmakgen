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
private:
  std::string path_;
  std::vector<std::shared_ptr<CmakeFunction>> functions_;
};

#endif