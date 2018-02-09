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

void CmakeFunctionArgument::addLineOffset(int offset) {
  line_ += offset;
}

void CmakeFunctionArgument::setColumn(int column) {
  column_ = column;
}

std::shared_ptr<CmakeFunction> CmakeFunction::create(std::string name, int line, std::vector<CmakeFunctionArgument> arguments) {
  return std::make_shared<CmakeFunction>(name, line, 0, line + arguments.size() - 1, 0, arguments);
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

void CmakeFunction::addLineOffset(int offset) {
  line_ += offset;
  endLine_ += offset;

  for (auto& arg : arguments_) {
    arg.addLineOffset(offset);
  }
}

void CmakeFunction::setEndLine(int line) {
  endLine_ = line;
}

void CmakeFunction::setArguments(std::vector<CmakeFunctionArgument> args) {
  arguments_ = args;
  updateEndColumn();
}

void CmakeFunction::updateEndColumn() {
  if (line_ == endLine_) {
    if (arguments_.empty()) {
      endColumn_ = column_ + 1;
    } else {
      const auto& lastArgument = arguments_[arguments_.size() - 1];
      endColumn_ = lastArgument.column() + lastArgument.value().size();
    }
  }
}