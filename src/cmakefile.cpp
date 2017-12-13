#include "../include/cmakefile.h"
#include <iostream>
#include <fstream>

namespace CmakeFile {
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
        stream << "set (SOURCE_FILES" << sourceFiles << "\n)\n\n";
      }

      const auto libraryProject = project.projectType() == "lib";
      if (libraryProject) {
        stream << "add_library (" << project.name();
      } else {
        stream << "add_executable (" << project.name();
      }

      if (hasIncludeFiles) { stream << " ${INCLUDE_FILES}"; }
      if (hasSourceFiles) { stream << " ${SOURCE_FILES}"; } 
      stream << ")\n\n";

      if (libraryProject) {
        stream << "install (DIRECTORY include/ DESTINATION include/" << project.name() << ")\n";
        stream << "install (\n\tTARGETS\n\t" << project.name() <<"\n\tRUNTIME DESTINATION bin\n\tLIBRARY DESTINATION lib\n\tARCHIVE DESTINATION lib/static\n)";
      }

      // dependancies
      
    }
  }

  CmakeProject load(std::shared_ptr<std::ifstream> fileStream) {
    return CmakeProject::createBaseProject("", "", BaseCmakeSettings{"", "", ""}, {}, {});
  }
}