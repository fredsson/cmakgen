#include <iostream>
/*#include "../include/cmakeproject.h"*/
#include "../include/cmakegenerator.h"
#include "../include/fileutils.h"
/*#include "../include/cmakefile.h"*/
#include <map>
/*#include "../include/commandlineutils.h"*/
#include "../include/cmakefile.h"
#include "../include/parser/cmakeparser.h"
#include "../include/writer/cmakewriter.h"
/*#include "../include/parser/cmlistfilelexer.h"*/

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

/*void readCmakeFile(const std::string& fileName) {
  cmListFileLexer* lexer = cmListFileLexer_New();

  if (!cmListFileLexer_SetFileName(lexer, fileName.c_str(), nullptr)) {
    cmListFileLexer_Delete(lexer);
    std::cout << "did not find cmake file" << "\n";
    return;
  }

  // read more here
  //https://cgit.kde.org/kdevelop.git/tree/projectmanagers/cmake/parser/cmakelistsparser.cpp?id=95497835ee1c721648d34667dd4147d339e8e4c4
  bool readError = false, haveNewline = true;
  cmListFileLexer_Token* token;
  while(!readError && (token = cmListFileLexer_Scan(lexer))) {
    readError=false;
    if(token->type == cmListFileLexer_Token_Newline) {
        readError=false;
        haveNewline = true;
    } else if (token->type == cmListFileLexer_Token_Identifier) {
      if (haveNewline) {
        haveNewline = false;
        const auto text = std::string(token->text);
        const auto line = token->line;
        const auto column = token->column;
        std::cout << "found function name " << text << " at " << line << ":" << column << "\n";
        
        cmListFileLexer_Token* token2;
        if(!(token2 = cmListFileLexer_Scan(lexer))) {
          std::cout << "did not find token\n";
          return;
        }

        while (token2->type == cmListFileLexer_Token_Space) {
          if(!(token2 = cmListFileLexer_Scan(lexer))) {
            std::cout << "did not find token\n";
            return;
          }
        }

        if(token2->type != cmListFileLexer_Token_ParenLeft) {
          std::cout << "did not find left parantesis\n";
          return;
        }

        int parenthesis=1;
        while((token2 = cmListFileLexer_Scan(lexer))) {
          switch(token2->type) {
            case cmListFileLexer_Token_ParenRight:
              parenthesis--;
              if(parenthesis==0) {
                std::cout << "end at: " << token2->line << ":" << token2->column << "\n";
                return;
              } else if (parenthesis < 0) {
                std::cout << "bad function\n";
                return;
              } else {
                std::cout << "argument: " << token2->text << "\n"; 
              }
              break;
            case cmListFileLexer_Token_ParenLeft:
              parenthesis++;
              std::cout << "argument: " << token2->text << "\n";
              break;
            case cmListFileLexer_Token_Identifier:
            case cmListFileLexer_Token_ArgumentUnquoted:
              std::cout << "argument: " << token2->text << "\n";
              break;
            case cmListFileLexer_Token_Space:
              continue;
            default:
              return;
          }
        }
      }
    }
  }
}

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
}*/

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
  const auto cmakeFile = generator.run();
  if (cmakeFile) {
    CmakeWriter::write(*cmakeFile);
  }

 /*auto generator = CmakeGenerator{ioHandler, {file_utils::currentDirName(), defaultCmake, defaultCppVersion, file_utils::getSubProjectDirectories()}};
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
  }*/
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