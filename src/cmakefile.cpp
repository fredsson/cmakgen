#include "../include/cmakefile.h"
#include "../include/cmakefunction.h"
#include <algorithm>
#include <iostream>

namespace {

bool contains(const std::string& target, const std::string& search) {
  return target.find(search) != std::string::npos;
}

bool functionMatches(const std::shared_ptr<CmakeFunction>& f, const std::string& functionName, const std::string& argument) {
  if (!contains(f->name(), functionName) || f->arguments().empty()) {
    return false;
  }

  return std::any_of(f->arguments().begin(), f->arguments().end(), [argument](const CmakeFunctionArgument& arg) {
    return contains(arg.value(), argument);
  });
}

std::shared_ptr<CmakeFunction> findFunction(const std::vector<std::shared_ptr<CmakeFunction>>& functions, const std::string& functionName, const std::string& argument) {
  for (const auto& f : functions) {
    if (functionMatches(f, functionName, argument)) {
      return f;
    }
  }

  return nullptr;
}

std::shared_ptr<CmakeFunction> findProjectTarget(const std::vector<std::shared_ptr<CmakeFunction>>& functions, const std::string& argument) {
  const auto& target = findFunction(functions, "add_executable", argument);
  return target ? target : findFunction(functions, "add_library", argument);
}

int calculateFilesOffset(const std::shared_ptr<CmakeFunction>& filesFunction, const std::vector<std::string>& files) {
  int offsetForFirstArgument = (filesFunction->line() == filesFunction->arguments()[0].line()) ? 0 : 1;

  const auto offset = files.size() - (filesFunction->arguments().size() - offsetForFirstArgument) + 1;
  
  return offset;
}

int calculateIncludeFilesOffset(const std::shared_ptr<CmakeFunction>& includeFilesFunction, const std::vector<std::string>& includeFiles) {
  // no include files in project
  if (!includeFilesFunction && includeFiles.empty()) {
    return 0;
  }

  // include files added
  if (!includeFilesFunction && !includeFiles.empty()) {
    return includeFiles.size() + 3;
  }

  // all include files removed
  if (includeFilesFunction && includeFiles.empty()) {
    int offsetForFirstArgument = (includeFilesFunction->line() == includeFilesFunction->arguments()[0].line()) ? 0 : 1;
    return -(includeFilesFunction->arguments().size() + offsetForFirstArgument + 2);
  }

  return calculateFilesOffset(includeFilesFunction, includeFiles);
}

void updateArguments(const std::shared_ptr<CmakeFunction>& f, const std::vector<std::string>& files) {
  std::vector<CmakeFunctionArgument> newArguments = {};
  newArguments.push_back(f->arguments()[0]);

  int line = f->arguments()[0].line() + 1;
  for (const auto& file : files) {
    newArguments.emplace_back(file, true, line++, 2);
  }
  f->setArguments(newArguments);
  f->setEndLine(line);
}

}

CmakeFile::CmakeFile(std::string path, std::vector<std::shared_ptr<CmakeFunction>> functions)
  : path_(path), functions_(functions) {
}

const std::string& CmakeFile::path() const {
  return path_;
}

const std::vector<std::shared_ptr<CmakeFunction>>& CmakeFile::functions() const {
  return functions_;
}

void CmakeFile::updateFiles(const std::vector<std::string>& includeFiles, const std::vector<std::string>& srcFiles) {
  const auto includeFilesFunction = findFunction(functions_, "set", "INCLUDE_FILES");
  const auto srcFilesFunction = findFunction(functions_, "set", "SRC_FILES");

  const auto includeFilesOffset = calculateIncludeFilesOffset(includeFilesFunction, includeFiles);
  const auto srcOffset = calculateFilesOffset(srcFilesFunction, srcFiles);

  bool setIncludeFilesOffset = false;
  if (includeFiles.empty() && includeFilesFunction) {
    setIncludeFilesOffset = true;
    removeIncludeFilesFunction();
  } else if (!includeFiles.empty() && !includeFilesFunction) {
    setIncludeFilesOffset = true;
    addIncludeFilesFunction(srcFilesFunction, includeFiles);
  }
  
  int lineOffset = 0;
  for (const auto& f : functions_) {    
    if (functionMatches(f, "set", "INCLUDE_FILES")) {
      updateArguments(f, includeFiles);
      lineOffset = includeFilesOffset;
    } else if (functionMatches(f, "set", "SRC_FILES")) {
      if (setIncludeFilesOffset) {
        lineOffset = includeFilesOffset;
      }
      updateArguments(f, srcFiles);
      f->addLineOffset(lineOffset);
      lineOffset += srcOffset;
    } else {
      f->addLineOffset(lineOffset);
    }
  }
}

void CmakeFile::removeIncludeFilesFunction() {
  functions_.erase(std::remove_if(functions_.begin(), functions_.end(), [](const auto& f) {
      return functionMatches(f, "set", "INCLUDE_FILES");
  }));

  const auto& target = findProjectTarget(functions_, "INCLUDE_FILES");
  if (!target) {
    return;
  }

  int newColumnPosition = 0;
  std::vector<CmakeFunctionArgument> shiftedArguments = {};
  std::transform(target->arguments().begin(), target->arguments().end(), std::back_inserter(shiftedArguments), [&newColumnPosition](const auto& argument) {
    const auto argumentColumn = newColumnPosition;
    if (newColumnPosition || contains(argument.value(), "INCLUDE_FILES")) {
      newColumnPosition = argument.column();
    }

    return CmakeFunctionArgument{argument.value(), argument.quoted(), argument.line(), (argumentColumn ? argumentColumn : argument.column())};
  });

  std::vector<CmakeFunctionArgument> arguments = {};
  std::copy_if(shiftedArguments.begin(), shiftedArguments.end(), std::back_inserter(arguments), [](const auto& argument) {
      return !contains(argument.value(), "INCLUDE_FILES");
  });

  target->setArguments(arguments);
}

void CmakeFile::addIncludeFilesFunction(const std::shared_ptr<CmakeFunction>& srcFilesFunction, const std::vector<std::string>& includeFiles) {
  std::vector<CmakeFunctionArgument> arguments = {};
  arguments.push_back({"INCLUDE_FILES", false, srcFilesFunction->line(), 5});

  int line = srcFilesFunction->line() + 1;
  std::transform(includeFiles.begin(), includeFiles.end(), std::back_inserter(arguments), [&line](const std::string& file) {
    return CmakeFunctionArgument{file, true, line++, 2};
  });
  functions_.insert(std::find(functions_.begin(), functions_.end(), srcFilesFunction), CmakeFunction::create("set",srcFilesFunction->line(),arguments));

  const auto& target = findProjectTarget(functions_, "SRC_FILES");
  if (!target) {
    return;
  }

  std::vector<CmakeFunctionArgument> newArguments = {};
  const int includeFilesArgumentSize = 17;
  bool includeFilesArgumentInserted = false;
  for (int i = target->arguments().size() - 1; i >= 0; i--) {
    const auto& argument = target->arguments()[i];

    if (contains(argument.value(), "SRC_FILES")) {
      newArguments.push_back(CmakeFunctionArgument{argument.value(), argument.quoted(), argument.line(), argument.column() + includeFilesArgumentSize});
      newArguments.push_back(CmakeFunctionArgument{"${INCLUDE_FILES}", false, argument.line(), argument.column()});
      includeFilesArgumentInserted = true;
      continue;
    }

    const int column = includeFilesArgumentInserted ? argument.column() : argument.column() + includeFilesArgumentSize;

    newArguments.push_back(CmakeFunctionArgument{argument.value(), argument.quoted(), argument.line(), column});
  }
  
  target->setArguments({newArguments.crbegin(), newArguments.crend()});
}