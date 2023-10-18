#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "Token.h"
#include "TokenAdd.h"
#include "TokenByte.h"
#include "TokenInstr.h"
#include <map>
#include <regex>
#include <string>
#include <vector>

class Assembly {
public:
  Assembly();

  void readFile(const std::string &filePath);
  void passOne(const std::string &filePath);
  void stripComments();
  const std::vector<std::string> &getBuffer();
  Token *readToken(std::string &line);
  Token *createToken(const std::string tokenType, const std::string value,
                     const std::string op2);
  int getValidRegister(const std::string &item);
  unsigned int getSymbol(const std::string key);
  std::vector<Token *> getTokens();
  unsigned int offset = 4; // pc is 4 bytes
private:
  std::vector<std::string> file_buffer;
  std::vector<Token *> tokens;
  std::map<std::string, unsigned int> symbol_table;
};

#endif
