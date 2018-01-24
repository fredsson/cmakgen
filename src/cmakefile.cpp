#include "../include/cmakefile.h"

CmakeFile::CmakeFile(std::string path, std::vector<std::shared_ptr<CmakeFunction>> functions)
  : path_(path), functions_(functions) {
}

const std::string& CmakeFile::path() const {
  return path_;
}

const std::vector<std::shared_ptr<CmakeFunction>>& CmakeFile::functions() const {
  return functions_;
}