#include "../include/cmakegenerator.h"
#include "../include/fileutils.h"
#include "../include/cmakefile.h"
#include "../include/cmakefunction.h"
#include <algorithm>
#include "../include/parser/cmakeparser.h"

namespace {

std::string lowerCaseInput(IoHandler& ioHandler) {
  auto input = ioHandler.input();
  std::transform(input.begin(), input.end(), input.end(), ::tolower);
  return input;
}

std::string getOptionalInput(const std::string& input, const std::string& defaultValue) {
  return input.empty() ? defaultValue : input;
}

bool confirmationInput(IoHandler& ioHandler) {
  auto input = lowerCaseInput(ioHandler);
  while (input != "y" && input != "n") {
    input = lowerCaseInput(ioHandler);
  }
  return input == "y" ? true : false;
}

std::string getProjectType(IoHandler& ioHandler) {
  auto input = lowerCaseInput(ioHandler);
  while (input != "lib" && input != "exe") {
    input = input = lowerCaseInput(ioHandler);
  }

  return input;
}

void generateCmakeFile(
  IoHandler& ioHandler,
  const std::string& name,
  const std::string& cmakeVersion,
  const std::string& cppVersion,
  const std::vector<std::string>& subProjectNames,
  bool subProject
) {
  const auto filePath = subProject ? name + "/CMakeLists.txt" : "CMakeLists.txt";
  std::ofstream stream(filePath);

  if (!subProject) {
    ioHandler.write("CMake version? (" + cmakeVersion + ")");
    stream << "cmake_minimum_required(VERSION " << getOptionalInput(ioHandler.input(), cmakeVersion) << ")\n\n";
  }

  ioHandler.write("What is the project called? (" + name + ")");
  stream << "project(" << getOptionalInput(ioHandler.input(), name) << ")\n\n";

  if (!subProject) {
    ioHandler.write("C++ version? (" + cppVersion + ")");
    const auto usedCppVersion = getOptionalInput(ioHandler.input(), cppVersion);
    stream << "include(CheckCXXCompilerFlag)\ncheck_cxx_compiler_flag(\"-std=" << usedCppVersion << "\" COMPILER_SUPPORT_CPP)\n";
    stream << "if(COMPILER_SUPPORT_CPP)\n  set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std="<< usedCppVersion <<"\")\nendif()\n\n";
  }

  const auto includeFiles = file_utils::getIncludeFiles(subProject ? name : ".");
  const auto srcFiles = file_utils::getSourceFiles(subProject ? name : ".");

  if (!includeFiles.empty()) {
    stream << "set(INCLUDE_FILES\n";
    for (const auto& file : includeFiles) { stream << "  \"" << file << "\"\n"; }
    stream << ")\n\n";
  }

  if (!srcFiles.empty()) {
    stream << "set(SRC_FILES\n";
    for (const auto& file : srcFiles) { stream << "  \"" << file << "\"\n";}
    stream << ")\n\n";
  }

  for (const auto& subProjectName : subProjectNames) {
    stream << "add_subdirectory(" << subProjectName << ")\n\n";
  }
  
  if (subProjectNames.empty() || !includeFiles.empty() || !srcFiles.empty()) {
    ioHandler.write("Project type? (lib/exe)");
    const auto projectType = getProjectType(ioHandler);
    stream << (projectType.find("lib") != std::string::npos ? "add_library(" + name  : "add_executable(" + name);
    if (!includeFiles.empty()) { stream << " ${INCLUDE_FILES}"; }
    if (!srcFiles.empty()) { stream << " ${SRC_FILES}"; }

    stream << ")\n";
  } 
}

}

GeneratorSettings::GeneratorSettings(std::string defaultProjectName, std::string installedCmakeVersion, std::string defaultCppVersion, std::vector<std::string> subProjectNames)
  : defaultProjectName_(defaultProjectName), installedCmakeVersion_(installedCmakeVersion), defaultCppVersion_(defaultCppVersion), subProjectNames_(subProjectNames) {
}

const std::string& GeneratorSettings::defaultProjectName() const {
  return defaultProjectName_;
}

const std::string& GeneratorSettings::installedCmakeVersion() const {
  return installedCmakeVersion_;
}

const std::string& GeneratorSettings::defaultCppVersion() const {
  return defaultCppVersion_;
}

const std::vector<std::string>& GeneratorSettings::subProjectNames() const {
  return subProjectNames_;
}

CmakeGenerator::CmakeGenerator(IoHandler& ioHandler, GeneratorSettings settings)
  : ioHandler_(ioHandler), settings_(settings) {
}

void CmakeGenerator::run() {
  std::ofstream stream ("CMakeLists.txt");
  if (!stream.is_open()) {
    ioHandler_.write("Could not open CMakeLists.txt for writing\n");
    return;
  }

  ioHandler_.write("Welcome to the cmakgen\n");
  ioHandler_.write("This tool will guide you through the process of configuring all the CMakeLists.txt files needed for your cpp project\n");

  generateCmakeFile(ioHandler_, settings_.defaultProjectName(), settings_.installedCmakeVersion(), settings_.defaultCppVersion(), settings_.subProjectNames(), false);

  for (const auto& subProjectName : settings_.subProjectNames()) {
    ioHandler_.write("Found src folder for " + subProjectName + ", include it in cmake file? (y/n)");
    if (confirmationInput(ioHandler_)) {
      generateCmakeFile(ioHandler_, subProjectName, "", "", {}, true);      
    }
  }
}