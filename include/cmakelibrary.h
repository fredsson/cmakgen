#ifndef CMAKE_LIBRARY_H
#define CMAKE_LIBRARY_H
#include <vector>
#include <string>
#include <optional>

class CmakeLibrary {
public:
  CmakeLibrary(std::string name, bool internal, std::vector<std::string> options);

  const std::string& name() const;
  bool internal() const;
  std::optional<std::string> location() const;
  const std::vector<std::string>& options() const;
  
  void setLocation(std::string location);
private:
  std::string name_;
  bool internal_;
  std::optional<std::string> location_;
  std::vector<std::string> options_;

};

#endif