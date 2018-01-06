#ifndef CMAKE_PACKAGE_H
#define CMAKE_PACKAGE_H
#include <vector>
#include <string>
#include <optional>

class CmakePackage {
public:
  CmakePackage(std::string name, std::vector<std::string> preOptions, std::vector<std::string> options, std::vector<std::string> modules);

  const std::string& name() const;
  const std::vector<std::string>& preOptions() const;
  const std::vector<std::string>& options() const;
  const std::vector<std::string>& modules() const;

private:
  std::string name_;
  std::vector<std::string> preOptions_;
  std::vector<std::string> options_;
  std::vector<std::string> modules_;
};

#endif