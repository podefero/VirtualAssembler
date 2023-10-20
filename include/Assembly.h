#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "Token.h"
#include "TokenAdd.h"
#include "TokenByte.h"
#include "TokenDiv.h"
#include "TokenInstr.h"
#include "TokenInt.h"
#include "TokenJmp.h"
#include "TokenLdb.h"
#include "TokenLdr.h"
#include "TokenMove.h"
#include "TokenMul.h"
#include "TokenStb.h"
#include "TokenStr.h"
#include "TokenSub.h"
#include "TokenTrap.h"
#include <map>
#include <regex>
#include <string>
#include <vector>

class Assembly {
public:
  Assembly();

  void readFile(const std::string &filePath);
  void passOne(const std::string &filePath);
  void passTwo();
  void stripComments();
  const std::vector<std::string> &getBuffer();
  Token *readToken(std::string &line);
  Token *createToken(const std::string tokenType, const std::string value,
                     const std::string op2);
  int getValidRegister(const std::string &item);
  int getImmediate(const std::string &item);
  unsigned int getSymbol(const std::string key);
  std::vector<Token *> getTokens();
  unsigned int offset = 4; // pc is 4 bytes t
  unsigned int data_seg_end;
  std::vector<unsigned char> bin_file;
  std::string toHex(const std::vector<unsigned char> &data);

private:
  std::vector<std::string> file_buffer;
  std::vector<Token *> tokens;
  std::map<std::string, unsigned int> symbol_table;
  bool done_instruction = false;
  bool found_trap0 = false;
};

#endif
