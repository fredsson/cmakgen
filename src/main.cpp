#include <iostream>
#include "../include/cmakeproject.h"
#include "../include/cmakegenerator.h"
#include "../include/fileutils.h"
#include "../include/cmakefile.h"
#include <map>
#include "../include/commandlineutils.h"


class StdIoHandler : public IoHandler {
public:
  StdIoHandler() {}
  void write(const std::string& text) override {
    std::cout << text << "\n";
  }

  std::string input() {
    std::string input;
    std::getline(std::cin, input);
    return input;
  }
};

void updateProjectFiles(CmakeProject& project) {
  if (project.projectType().has_value()) {
    project.setIncludeFiles(file_utils::getIncludeFiles(project.name()));
    project.setSourceFiles(file_utils::getSourceFiles(project.name()));
  }

  for (auto& subProject : project.subProjects()) {
    if (subProject.projectType().has_value()) {
      subProject.setIncludeFiles(file_utils::getIncludeFiles(subProject.name()));
      subProject.setSourceFiles(file_utils::getSourceFiles(subProject.name()));
    }
  }
}

void buildProject() {
  file_utils::createDir("_build");

  const auto file = std::shared_ptr<std::ifstream>(new std::ifstream("CMakeLists.txt"));
  const auto project = CmakeFile::load(file, false);
  if (project.has_value()) {
    auto loadedProject = project.value();
    updateProjectFiles(loadedProject);
    CmakeFile::save(file_utils::createCmakeFile(), loadedProject);
  }
  commandline_utils::executeBuildCommand();
}

void generateCmake(IoHandler& ioHandler, const std::string& defaultCmake, const std::string& defaultCppVersion) {
  auto generator = CmakeGenerator{ioHandler, {file_utils::currentDirName(), defaultCmake, defaultCppVersion, file_utils::getSubProjectDirectories()}};
  const auto project = generator.run();
  auto baseFile = file_utils::createCmakeFile();
  if (baseFile) {
    CmakeFile::save(baseFile, project);
    baseFile->close();
  }
    
  for (const auto& subProject : project.subProjects()) {
    auto file = file_utils::createCmakeFile(subProject.name());
    if (file) {
      CmakeFile::save(file, subProject);
      file->close();
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "no arguments provided\n";
    return 0;
  }

  std::map<std::string, std::string> options;
  for (int i = 2; i < argc; i++) {
    const auto option = std::string(argv[i]);
      options.insert(std::make_pair(option.substr(0, option.find("=")), option.substr(option.find("=") + 1)));
  }

  auto ioHandler = StdIoHandler{};

  const auto command = std::string(argv[1]);
  if (command == "--build" || command == "-b") {
    buildProject();
  } else if (command == "--gen" || command == "-g") {
    auto cmakeIt = options.find("--cmake");
    auto cppIt = options.find("--cpp");
    generateCmake(ioHandler, (cmakeIt != options.end() ? cmakeIt->second : "3.10.0"), (cppIt != options.end() ? cppIt->second : "c++11"));
  } else if (command == "--dep" || command == "-d") {
    std::cout << "received dep command\n";
  } else {
    // print help
  }

  return 0;
}