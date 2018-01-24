#ifndef CMAKEPARSER_H
#define CMAKEPARSER_H
#include <memory>
#include <string>

class CmakeFile;
namespace CmakeParser {
std::shared_ptr<CmakeFile> parse(const std::string& fileName);
}

#endif