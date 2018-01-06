#include "../include/cmakefile.h"
#include "../include/fileutils.h"
#include "../include/cmakepackage.h"
#include "../include/cmakelibrary.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>

namespace CmakeFile {

namespace {

const std::regex MINIMUM_VERSION_EXPRESSION = std::regex("cmake_minimum_required.*VERSION.*((\\d+\\.)(\\d+\\.)(\\d+))\\)");
const std::regex PROJECT_NAME_EXPRESSION = std::regex("project.*\\((.*)\\)");
const std::regex PROJECT_BUILD_TYPE_EXPRESSION = std::regex(".*\\(CMAKE_BUILD_TYPE (.*)\\).*");
const std::regex CPP_VERSION_EXPRESSION = std::regex(".*-std=(.*)\".*");
const std::regex SUB_PROJECT_NAME_EXPRESSION = std::regex("add_subdirectory.*\\((.*)\\)");
const std::regex INCLUDE_FILES_EXPRESSION = std::regex(".*INCLUDE_FILES.*");
const std::regex SOURCE_FILES_EXPRESSION = std::regex(".*SOURCE_FILES.*");
const std::regex INCLUDE_FILE_EXPRESSION = std::regex(".*\"include/(.*)\".*");
const std::regex SOURCE_FILE_EXPRESSION = std::regex(".*\"src/(.*)\".*");
const std::regex FILES_END_EXPRESSION = std::regex(".*\\).*");
const std::regex LIBRARY_EXPRESSION = std::regex("add_library.*");
const std::regex EXE_EXPRESSION = std::regex("add_executable.*");
const std::regex FIND_PACKAGE_LIBRARY_EXPRESSION = std::regex("find_package.*");
const std::regex FIND_LIBRARY_EXPRESSION = std::regex("add_library.*IMPORTED.*");
const std::regex FIND_LIBRARY_IMPORT_EXPRESSION = std::regex("set_property\\(TARGET.*PROPERTY IMPORTED_LOCATION.*");
const std::regex LINK_TARGET_EXPRESSION = std::regex("target_link_libraries.*");

std::vector<std::string> split(const std::string& subject) {
  static const std::regex re{"\\s+"};
  return {
    std::sregex_token_iterator(subject.begin(), subject.end(), re, -1), 
    std::sregex_token_iterator()
  };
}

std::string upperCase(std::string subject) {
  std::transform(subject.begin(), subject.end(), subject.begin(), ::toupper);
  return subject;
}

std::optional<std::string> extractWithExpression(const std::string& content, const std::regex& expression) {
  try {
    std::smatch match;
    if (std::regex_match(content, match, expression) && match.size() >= 2) {
      return match[1].str();
    }
  } catch (const std::regex_error& e) {
    std::cout << "regex_error caught: " << e.what() << " code: " << e.code() << '\n';
    return std::nullopt;
  }

  return std::nullopt;
}

std::optional<CmakePackage> extractPackage(const std::string& content) {
  
  const std::regex parameterRegex = std::regex("find_package.*\\((.*)\\)");
  const std::regex versionRegex = std::regex("^(\\d+).*");

  std::optional<std::string> extractedParameters = extractWithExpression(content, parameterRegex);
  if (extractedParameters.has_value()) {
    const auto parameters = split(extractedParameters.value());
    if (parameters.empty()) {
      std::cout << "could not find any parameters for package\n";
      return std::nullopt;
    }

    std::string name = parameters[0];
    std::vector<std::string> preOptions = {};
    std::vector<std::string> options = {};
    std::vector<std::string> modules = {};
    
    bool componentsAdded = false;
    for (auto i = 1; i < parameters.size(); i++) {
      if (componentsAdded) {
        options.push_back(parameters[i]);
      } else if (std::regex_match(parameters[i], versionRegex) 
        || parameters[i].find("EXACT") != std::string::npos
        || parameters[i].find("QUIET") != std::string::npos
        || parameters[i].find("REQUIRED") != std::string::npos
      ) {
        preOptions.push_back(parameters[i]);
      } else if (parameters[i].find("CONFIG") != std::string::npos || parameters[i].find("NO_MODULE") != std::string::npos) {
        componentsAdded = true;
        options.push_back(parameters[i]);
      } else {
        modules.push_back(parameters[i]);
      }
    }

    return CmakePackage{name, preOptions, options, modules};
      
  }

  return std::nullopt;
}

std::optional<CmakeLibrary> extractLibrary(const std::string& content) {
  const std::regex parameterRegex = std::regex("add_library.*\\((.*)\\)");

  std::optional<std::string> extractedParameters = extractWithExpression(content, parameterRegex);
  if (extractedParameters.has_value()) {
    const auto parameters = split(extractedParameters.value());
    if (parameters.empty()) {
      std::cout << "could not find any parameters for library\n";
      return std::nullopt;
    }

    return CmakeLibrary{parameters[0], false, {parameters.begin() + 1, parameters.end()}};
  }

  return std::nullopt;
}

}

void save(std::shared_ptr<std::ofstream> fileStream, const CmakeProject& project) {
  if (!fileStream) {
    std::cout << "Unable to save cmake to file, file does not exist\n";
    return;
  }

  auto& stream = *fileStream;
  const auto writeBaseSettings = project.baseSettings().has_value();

  if (writeBaseSettings) {
    stream << "cmake_minimum_required (VERSION " << project.baseSettings()->cmakeVersion() << ")\n\n";      
  }
  stream << "project (" << project.name() << ")\n\n";

  if (writeBaseSettings) {
    stream << "if (NOT CMAKE_BUILD_TYPE)\n\tset (CMAKE_BUILD_TYPE " << project.baseSettings()->projectBuildType() << ")\nendif (NOT CMAKE_BUILD_TYPE)\n\n";
    stream << "include (CheckCXXCompilerFlag)\nCHECK_CXX_COMPILER_FLAG(\"-std=" 
      << project.baseSettings()->cppVersion() 
      << "\" COMPILER_SUPPORT_CPP)\nif (COMPILER_SUPPORT_CPP)\n\tset (CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std=" 
      << project.baseSettings()->cppVersion() << "\")\nendif (COMPILER_SUPPORT_CPP)\n\n";
  }

  for (const auto& subProject : project.subProjects()) {
    stream << "add_subdirectory (" << subProject.name() << ")\n\n";
  }

  for (const auto& library : project.libraries()) {
    if (!library.internal()) {
      stream << "add_library(" << library.name();
      for (const auto& option : library.options()) { stream << " " << option; }
      stream << ")\n";
      if (library.location().has_value()) {
        stream << "set_property (TARGET " << library.name() << " PROPERTY IMPORTED_LOCATION " << library.location().value() << ")\n\n";
      } else {
        stream << "\n";
      }
    }
  }

  if (project.includeRootDirectory()) {
    stream << "include_directories (\"${CMAKE_SOURCE_DIR}\")\n\n"; 
  }

  if (project.projectType().has_value()) {
    const auto hasIncludeFiles = !project.includeFiles().empty();
    const auto hasSourceFiles = !project.sourceFiles().empty();

    if (hasIncludeFiles) {
      std::string includeFiles;
      for (const auto& includeFile : project.includeFiles()) { includeFiles += "\n\t\"include/" + includeFile + "\""; }
      stream << "set (INCLUDE_FILES" << includeFiles << "\n)\n\n";
    }

    if (hasSourceFiles) {
      std::string sourceFiles;
      for (const auto& sourceFile : project.sourceFiles()) { sourceFiles += "\n\t\"src/" + sourceFile + "\""; }
      stream << "set (SRC_FILES" << sourceFiles << "\n)\n\n";
    }

      for (const auto& package : project.packages()) {
        stream << "find_package (" << package.name();
        for (const auto& preOption : package.preOptions()) { stream << " " << preOption; }
        for (const auto& module : package.modules()) { stream << " " << module; }
        for (const auto& option : package.options()) { stream << " " << option; }
        stream << ")\n";
        stream << "include_directories (${" << upperCase(package.name()) << "_INCLUDE_DIR})\n\n";
      }

    const auto libraryProject = project.projectType() == "lib";
    if (libraryProject) {
      stream << "add_library (" << project.name();
    } else {
      stream << "add_executable (" << project.name();
    }

    if (hasIncludeFiles) { stream << " ${INCLUDE_FILES}"; }
    if (hasSourceFiles) { stream << " ${SRC_FILES}"; } 
    stream << ")\n\n";

    if (!project.packages().empty() || !project.libraries().empty()) {
      stream << "target_link_libraries (" << project.name();
      for (const auto& library : project.libraries()) {
        stream << " " << library.name();
      }
      for (const auto& package : project.packages()) {
        const auto upperCaseName = upperCase(package.name());
        stream << " ${" << upperCaseName << "_LIBRARIES} ${" << upperCaseName << "_DEPENDENCIES}";
      }
      stream << ")\n\n";
    }

    if (libraryProject) {
      stream << "install (DIRECTORY include/ DESTINATION include/" << project.name() << ")\n";
      stream << "install (\n\tTARGETS\n\t" << project.name() <<"\n\tRUNTIME DESTINATION bin\n\tLIBRARY DESTINATION lib\n\tARCHIVE DESTINATION lib/static\n)";
    }
  }

  for (const auto& subProject : project.subProjects()) {
    CmakeFile::save(file_utils::createCmakeFile(subProject.name()), subProject);
  }
}

std::optional<CmakeProject> load(std::shared_ptr<std::ifstream> fileStream, bool hasParent) {
  if (!fileStream) {
    std::cout << "Unable to load cmake file\n";
    return std::nullopt;
  }

  std::optional<std::string> name;
  std::optional<std::string> projectType;
  std::optional<std::string> projectBuildType;
  std::optional<std::string> cmakeVersion;
  std::optional<std::string> cppVersion;
  std::vector<std::string> includeFiles = {};
  std::vector<std::string> sourceFiles = {};
  std::vector<std::string> subProjectNames = {};
  std::vector<CmakePackage> packages = {};
  std::vector<CmakeLibrary> libraries = {};

  bool addIncludeFiles = false;
  bool addSourceFiles = false;
  auto& stream = *fileStream;
  std::string content;
  while (std::getline(stream, content)) {
    if (!cmakeVersion.has_value() ) {
      cmakeVersion = extractWithExpression(content, MINIMUM_VERSION_EXPRESSION);
    }

    if (!name.has_value()) {
      name = extractWithExpression(content, PROJECT_NAME_EXPRESSION);
    }

    if (!projectBuildType.has_value()) {
      projectBuildType = extractWithExpression(content, PROJECT_BUILD_TYPE_EXPRESSION);
    }

    if (!cppVersion.has_value()) {
      cppVersion = extractWithExpression(content, CPP_VERSION_EXPRESSION);
    }

    if (!projectType.has_value()) {
      if (std::regex_match(content, LIBRARY_EXPRESSION)) {
        projectType = std::make_optional("lib");
      } else if (std::regex_match(content, EXE_EXPRESSION)) {
        projectType = std::make_optional("exe");
      }
    }

    auto subProjectName = extractWithExpression(content, SUB_PROJECT_NAME_EXPRESSION);
    if (subProjectName.has_value()) {
      subProjectNames.push_back(subProjectName.value());
    }

    if (std::regex_match(content, INCLUDE_FILES_EXPRESSION)) {
      addIncludeFiles = true;
    }

    if (addIncludeFiles) {
      const auto file = extractWithExpression(content, INCLUDE_FILE_EXPRESSION);
      if (file.has_value()) {
        includeFiles.push_back(file.value());
      }
    }

    if (addIncludeFiles && std::regex_match(content, FILES_END_EXPRESSION)) {
      addIncludeFiles = false;
      continue;
    }

    if (std::regex_match(content, SOURCE_FILES_EXPRESSION)) {
      addSourceFiles = true;
    }

    if (addSourceFiles) {
      const auto file = extractWithExpression(content, SOURCE_FILE_EXPRESSION);
      if (file.has_value()) {
        sourceFiles.push_back(file.value());
      }
    }

    if (addSourceFiles && std::regex_match(content, FILES_END_EXPRESSION)) {
      addSourceFiles = false;
      continue;
    }

    if (std::regex_match(content, FIND_PACKAGE_LIBRARY_EXPRESSION)) {
      auto extractedPackage = extractPackage(content);
      if (extractedPackage.has_value()) {
        packages.push_back(extractedPackage.value());
      }
    }

    if (std::regex_match(content, FIND_LIBRARY_EXPRESSION)) {
      auto extractedLibrary = extractLibrary(content);
      if (extractedLibrary.has_value()) {
        libraries.push_back(extractedLibrary.value());
      }
    }

    if (std::regex_match(content, FIND_LIBRARY_IMPORT_EXPRESSION)) {
      const std::regex parameterRegex = std::regex("set_property.*\\((.*)\\)");
      std::optional<std::string> extractedParameters = extractWithExpression(content, parameterRegex);
      if (extractedParameters.has_value()) {
        const auto parameters = split(extractedParameters.value());
        for (auto& library : libraries) {
          if (library.name().find(parameters[0]) != std::string::npos) {
            library.setLocation(parameters[parameters.size() - 1]);
          }
        }
      }
    }

    if (std::regex_match(content, LINK_TARGET_EXPRESSION)) {
      const std::regex parameterRegex = std::regex("target_link_libraries.*\\((.*)\\)");
      std::optional<std::string> extractedParameters = extractWithExpression(content, parameterRegex);
      if (extractedParameters.has_value()) {
        const auto parameters = split(extractedParameters.value());
        for (auto i = 1; i < parameters.size(); i++) {
          const auto isPackage = std::any_of(packages.begin(), packages.end(), [param = parameters[i]](const CmakePackage& package) {
            return package.name().find(param) != std::string::npos;
          });
          const auto isLibrary = std::any_of(libraries.begin(), libraries.end(), [param = parameters[i]](const CmakeLibrary& library) {
            return library.name().find(param) != std::string::npos;
          });
          if (!isPackage && !isLibrary) {
            libraries.push_back(CmakeLibrary{parameters[i], true, {}});
          }
        }
      }
    }
  }

  if (!name.has_value()) {
    std::cout << "Loading failed - Could not find a name in the cmake file\n";
    return std::nullopt;
  }

  std::vector<CmakeProject> subProjects; 
  for (const auto& subProj : subProjectNames) {
    const auto file = std::shared_ptr<std::ifstream>(new std::ifstream(subProj + "/CMakeLists.txt"));
    const auto subProject = CmakeFile::load(file, true);
    if (subProject.has_value()) {
      subProjects.push_back(subProject.value());
    }
  }

  if (!hasParent) {
    const auto settings = BaseCmakeSettings{projectBuildType.value_or("Debug"), cmakeVersion.value_or("3.10.0"), cppVersion.value_or("c++11")};
    if (!subProjects.empty()) {
      return CmakeProject::createBaseProjectWithSubProjects(name.value(), settings, subProjects, packages, libraries);
    }

    if (!projectType.has_value()) {
      std::cout << "Loading failed - Could not find project type in cmake file\n";
      return std::nullopt;
    }

    return CmakeProject::createBaseProject(name.value(), projectType.value(), settings, includeFiles, sourceFiles, packages, libraries);
  }

  if (!projectType.has_value()) {
    std::cout << "Loading failed - Could not find project type in cmake file\n";
    return std::nullopt;
  }

  return CmakeProject::createSubProject(name.value(), projectType.value(), true, includeFiles, sourceFiles, packages, libraries);
}
}