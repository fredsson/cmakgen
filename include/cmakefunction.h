#ifndef CMAKEFUNCTION_H
#define CMAKEFUNCTION_H
#include <string>
#include <vector>
#include <memory>

class CmakeFunctionArgument {
public:
  CmakeFunctionArgument(std::string value, bool quoted, int line, int column);
  const std::string& value() const;
  bool quoted() const;
  int line() const;
  int column() const;

  void addLineOffset(int offset);
  void setColumn(int column);
private:
  std::string value_;
  bool quoted_;
  int line_;
  int column_;
};

class CmakeFunction {
public:
  static std::shared_ptr<CmakeFunction> create(std::string name, int line, std::vector<CmakeFunctionArgument> arguments);
  CmakeFunction(std::string name, int line, int column, int endLine, int endColumn, std::vector<CmakeFunctionArgument> arguments);

  const std::string& name() const;
  int line() const;
  int column() const;
  int endLine() const;
  int endColumn() const;
  const std::vector<CmakeFunctionArgument>& arguments() const;

  void addLineOffset(int offset);
  void setEndLine(int line);
  void setArguments(std::vector<CmakeFunctionArgument> args);
private:
  void updateEndColumn();
  std::string name_;
  int line_;
  int column_;
  int endLine_;
  int endColumn_;
  std::vector<CmakeFunctionArgument> arguments_;
};

#endif