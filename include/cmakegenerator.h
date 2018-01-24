#ifndef CMAKE_GENERATOR_H
#define CMAKE_GENERATOR_H
#include <memory>
#include <vector>


class IoHandler {
public:
  virtual void write(const std::string& text) = 0;
  virtual std::string input() = 0;
};

class GeneratorSettings {
public:
  GeneratorSettings(std::string defaultProjectName, std::string installedCmakeVersion, std::string defaultCppVersion, std::vector<std::string> subProjectNames);
  const std::string& defaultProjectName() const;
  const std::string& installedCmakeVersion() const;
  const std::string& defaultCppVersion() const;
  const std::vector<std::string>& subProjectNames() const;
private:
  std::string defaultProjectName_;
  std::string installedCmakeVersion_;
  std::string defaultCppVersion_;
  std::vector<std::string> subProjectNames_;
};

class CmakeFile;
class CmakeGenerator {
public:
  CmakeGenerator(IoHandler& ioHandler, GeneratorSettings settings);
  std::shared_ptr<CmakeFile> run();
private:
  IoHandler& ioHandler_;
  GeneratorSettings settings_;
};

#endif