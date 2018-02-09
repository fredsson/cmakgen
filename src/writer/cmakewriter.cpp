#include "../../include/writer/cmakewriter.h"
#include "../../include/cmakefile.h"
#include "../../include/cmakefunction.h"
#include <iostream>
#include <fstream>

namespace CmakeWriter {

struct FilePosition {
  int line;
  int column;
};

namespace {

FilePosition advanceToPosition(std::ofstream& stream, const FilePosition& current, const FilePosition& target) {
  auto currentLine = current.line;
  auto currentColumn = current.column;

  if (currentLine < target.line) {
    for (;currentLine < target.line; currentLine++) { stream << "\n"; }
    currentColumn = 1;
  }

  if (currentColumn < target.column) {
    for (;currentColumn < target.column; currentColumn++) { stream << " "; }
  }

  return {currentLine, currentColumn};
}

FilePosition writeFunctionArgument(std::ofstream& stream, const CmakeFunctionArgument& argument, const FilePosition& current) {
  const auto position = advanceToPosition(stream, current, { argument.line(), argument.column() });

  const auto quotations = (argument.quoted() ? "\"" : "");

  stream << quotations << argument.value() << quotations;

  const int argumentSize = argument.value().size() + (argument.quoted() * 2);

  return {position.line, position.column + argumentSize};
}

FilePosition writeFunction(std::ofstream& stream, const std::shared_ptr<CmakeFunction>& function, const FilePosition& currentPosition) {
  auto position = advanceToPosition(stream, currentPosition, {function->line(), function->column()});

  stream << function->name() << "(";

  position.column += function->name().size() + 1;

  for (const auto& argument : function->arguments()) {
    position = writeFunctionArgument(stream, argument, position);
  }

  position = advanceToPosition(stream, position, {function->endLine(), function->endColumn()});

  stream << ")";

  return position;
}
}

void write(const CmakeFile& file) {
  std::ofstream stream(file.path());
  if (!stream.is_open()) {
    std::cout << "Could not open file " << file.path() << " for reading.\n";
    return;
  }

  auto position = FilePosition{1, 1};
  for (const auto& function : file.functions()) {
    position = writeFunction(stream, function, position);
  }
}
}