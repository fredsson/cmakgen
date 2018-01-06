#include "../include/cmakegenerator.h"
#include "../include/cmakeproject.h"
#include "../include/fileutils.h"
#include <algorithm>

namespace {
std::string WELCOME_MESSAGE = "Welcome to the cmakgen\nThis tool will guide you through the process of configuring all the CMakeLists.txt files needed for your cpp project";
std::string PROJECT_NAME_QUESTION = "What is the project called?";
std::string PROJECT_CMAKE_VERSION_QUESTION = "CMake version?";
std::string PROJECT_CPP_VERSION_QESTION = "Default c++ version?";
std::string PROJECT_TYPE_QUESTION = "Project type? (exe/lib)";
std::string SUB_PROJECT_QUESTION_PRE = "Found src folder for ";
std::string SUB_PROJECT_QUESTION_SUB = ", include it in cmake file? (y/n)";

std::string lowerCaseInput(IoHandler& ioHandler) {
  auto input = ioHandler.input();
  std::transform(input.begin(), input.end(), input.end(), ::tolower);
  return input;
}

std::optional<std::string> getOptionalInput(IoHandler& ioHandler) {
  const auto input = ioHandler.input();
  return input.size() ? std::make_optional(input) : std::nullopt;
}

std::string getProjectType(IoHandler& ioHandler) {
  auto input = lowerCaseInput(ioHandler);
  while (input != "lib" && input != "exe") {
    input = input = lowerCaseInput(ioHandler);
  }

  return input;
}

bool confirmationInput(IoHandler& ioHandler) {
  auto input = lowerCaseInput(ioHandler);
  while (input != "y" && input != "n") {
    input = lowerCaseInput(ioHandler);
  }
  return input == "y" ? true : false;
}

CmakeProject createSubProject(IoHandler& ioHandler, const std::string& projectName) {
  ioHandler.write(PROJECT_TYPE_QUESTION);
  const auto projectType = getProjectType(ioHandler);
  return CmakeProject::createSubProject(projectName, projectType, true, file_utils::getIncludeFiles(projectName), file_utils::getSourceFiles(projectName), {}, {});
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

CmakeProject CmakeGenerator::run() {
  ioHandler_.write(WELCOME_MESSAGE);

  ioHandler_.write(PROJECT_NAME_QUESTION + " (" + settings_.defaultProjectName() + ")");
  const auto projectName = getOptionalInput(ioHandler_).value_or(settings_.defaultProjectName());

  ioHandler_.write(PROJECT_CMAKE_VERSION_QUESTION + " (" + settings_.installedCmakeVersion() + ")");
  const auto projectCmakeVersion = getOptionalInput(ioHandler_).value_or(settings_.installedCmakeVersion());

  ioHandler_.write(PROJECT_CPP_VERSION_QESTION + " (" + settings_.defaultCppVersion() + ")");
  const auto projectCppVersion = getOptionalInput(ioHandler_).value_or(settings_.defaultCppVersion());

  const auto baseSettings = BaseCmakeSettings{"Debug", projectCmakeVersion, projectCppVersion};
  if (settings_.subProjectNames().size() > 0) {
    std::vector<CmakeProject> subProjects = {};
    for (const auto& projectName : settings_.subProjectNames()) {
      ioHandler_.write(SUB_PROJECT_QUESTION_PRE + projectName + SUB_PROJECT_QUESTION_SUB);
      if (confirmationInput(ioHandler_)) {
        subProjects.push_back(createSubProject(ioHandler_, projectName));
      }
    }
    return CmakeProject::createBaseProjectWithSubProjects(projectName, baseSettings, subProjects, {}, {});
  } else {
    ioHandler_.write(PROJECT_TYPE_QUESTION);
    const auto projectType = getProjectType(ioHandler_);
    return CmakeProject::createBaseProject(projectName, projectType, baseSettings, {}, {}, {}, {});
  }
}