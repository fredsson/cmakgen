#include "../include/cmakefunction.h"

CmakeFunctionArgument::CmakeFunctionArgument(std::string value, bool quoted, int line, int column)
  : value_(value), quoted_(quoted), line_(line), column_(column) {
}

const std::string& CmakeFunctionArgument::value() const {
  return value_;
}

bool CmakeFunctionArgument::quoted() const {
  return quoted_;
}

int CmakeFunctionArgument::line() const {
  return line_;
}

int CmakeFunctionArgument::column() const {
  return column_;
}

CmakeFunction::CmakeFunction(std::string name, int line, int column, int endLine, int endColumn, std::vector<CmakeFunctionArgument> arguments)
  : name_(name), line_(line), column_(column), endLine_(endLine), endColumn_(endColumn), arguments_(arguments) {
}

const std::string& CmakeFunction::name() const {
  return name_;
}
int CmakeFunction::line() const {
  return line_;
}
int CmakeFunction::column() const {
  return column_;
}
int CmakeFunction::endLine() const {
  return endLine_;
}
int CmakeFunction::endColumn() const {
  return endColumn_;
}

const std::vector<CmakeFunctionArgument>& CmakeFunction::arguments() const {
  return arguments_;
}