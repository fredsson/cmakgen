#include "../../include/parser/cmakeparser.h"
#include "../../include/parser/cmlistfilelexer.h"
#include "../../include/cmakefile.h"
#include "../../include/cmakefunction.h"
#include <iostream>
#include <functional>

namespace {

std::string upperCase(std::string subject) {
  std::transform(subject.begin(), subject.end(), subject.begin(), ::toupper);
  return subject;
}

std::string lowerCase(std::string subject) {
  std::transform(subject.begin(), subject.end(), subject.begin(), ::tolower);
  return subject;
}

std::shared_ptr<CmakeFunction> parseCmakeFunction(cmListFileLexer* lexer, const std::string& functionName, int line, int column) {
  cmListFileLexer_Token* token = cmListFileLexer_Scan(lexer);

  int argumentStartColumn = 1;
  while (token->type == cmListFileLexer_Token_Space) {
    token =  cmListFileLexer_Scan(lexer);
    argumentStartColumn--;
  }

  if (!token || token->type != cmListFileLexer_Token_ParenLeft) {
    std::cout << "no token or no parenthesis" << "\n";
    return nullptr;
  }

  int parenthesis = 1;
  std::vector<CmakeFunctionArgument> arguments = {};
  while((token = cmListFileLexer_Scan(lexer))) {
    switch (token->type) {
      case cmListFileLexer_Token_ParenRight:
        parenthesis--;
        if (parenthesis == 0) {
          return std::make_shared<CmakeFunction>(lowerCase(functionName), line, column, token->line, token->column, arguments);
        } else if (parenthesis < 0) {
          return nullptr;
        } else {
          arguments.push_back({token->text, false, token->line, token->column + argumentStartColumn});
        }
        break;
      case cmListFileLexer_Token_ParenLeft:
        parenthesis++;
      case cmListFileLexer_Token_Identifier:
      case cmListFileLexer_Token_ArgumentUnquoted:
        arguments.push_back({token->text, false, token->line, token->column + argumentStartColumn});
        break;
      case cmListFileLexer_Token_ArgumentQuoted:
        arguments.push_back({token->text, true, token->line, token->column + argumentStartColumn});
        break;
      case cmListFileLexer_Token_Newline:
      case cmListFileLexer_Token_Space:
        break;
      default:
        std::cout << "Could not parse token type: " << token->type << " at " << token->line << ":" << token->column << "with text" << token->text << "\n";
        return nullptr; 
    }
  }

  return nullptr;
} 
}

namespace CmakeParser {

std::shared_ptr<CmakeFile> parse(const std::string& fileName) {
  cmListFileLexer* lexer = cmListFileLexer_New();
  if (!lexer) {
    return nullptr;
  }

  if (!cmListFileLexer_SetFileName(lexer, fileName.c_str(), nullptr)) {
    std::cout << "Could not read " << fileName << "\n";
    return nullptr; 
  }

  cmListFileLexer_Token* token;
  bool readError = false;
  bool hasNewline = true;

  std::vector<std::shared_ptr<CmakeFunction>> functions;
  while (!readError && (token = cmListFileLexer_Scan(lexer))) {
    readError = false;
    switch (token->type) {
      case cmListFileLexer_Token_Newline:
        readError = false;
        hasNewline = true;
        break;
      case cmListFileLexer_Token_Identifier: 
        if (hasNewline) {
          hasNewline = false;
          const auto function = parseCmakeFunction(lexer, token->text, token->line, token->column);
          if (function) {
            functions.push_back(function);
          }
        }
        break;
    }
  }

  cmListFileLexer_Delete(lexer);
  return std::make_shared<CmakeFile>(fileName, functions);
}

}