#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "Token.h"
#include "TokenByte.h"
#include <regex>
#include <string>
#include <vector>

class Assembly {
public:
  Assembly();

  void readFile(const std::string &filePath);

  void stripComments();
  std::vector<std::string> getBuffer();
  Token *readToken(std::string &line);
  std::string getTokenType(std::string &item);
  Token *createToken(std::string tokenType, std::string value, std::string op2);
  unsigned int offset = 4; // pc is 4 bytes
private:
  std::vector<std::string> file_buffer;
  std::vector<Token> *tokens;
};

#endif
