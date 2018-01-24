#ifndef CMAKEFUNCTION_H
#define CMAKEFUNCTION_H
#include <string>
#include <vector>

class CmakeFunctionArgument {
public:
  CmakeFunctionArgument(std::string value, bool quoted, int line, int column);
  const std::string& value() const;
  bool quoted() const;
  int line() const;
  int column() const;
private:
  std::string value_;
  bool quoted_;
  int line_;
  int column_;
};

class CmakeFunction {
public:
  CmakeFunction(std::string name, int line, int column, int endLine, int endColumn, std::vector<CmakeFunctionArgument> arguments);

  const std::string& name() const;
  int line() const;
  int column() const;
  int endLine() const;
  int endColumn() const;
  const std::vector<CmakeFunctionArgument>& arguments() const;
private:
  std::string name_;
  int line_;
  int column_;
  int endLine_;
  int endColumn_;
  std::vector<CmakeFunctionArgument> arguments_;
};

#endif