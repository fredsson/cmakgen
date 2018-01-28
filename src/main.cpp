#include <iostream>
/*#include "../include/cmakeproject.h"*/
#include "../include/cmakegenerator.h"
#include "../include/fileutils.h"
#include <map>
/*#include "../include/commandlineutils.h"*/
#include "../include/cmakefile.h"
#include "../include/cmakefunction.h"
#include "../include/parser/cmakeparser.h"
#include "../include/writer/cmakewriter.h"
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

void buildProject() {
  file_utils::createDir("_build");

  const auto cmakeFile = CmakeParser::parse("CMakeLists.txt");
  if (cmakeFile) {
    CmakeWriter::write(*cmakeFile);
  } else {
    std::cout << "could not load cmake file!\n";
  }

  for (const auto& subFolder : file_utils::getSubProjectDirectories()) {
   const auto cmakeFile = CmakeParser::parse(subFolder + "/CMakeLists.txt");
    if (cmakeFile) {
      CmakeWriter::write(*cmakeFile);
    } else {
      std::cout << "could not load cmake file!\n";
    } 
  }
  //commandline_utils::executeBuildCommand();
}

void generateCmake(IoHandler& ioHandler, const std::string& defaultCmake, const std::string& defaultCppVersion) {
  auto generator = CmakeGenerator(ioHandler, {file_utils::currentDirName(), defaultCmake, defaultCppVersion, file_utils::getSubProjectDirectories()});
  generator.run();
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