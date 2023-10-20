#ifndef TOKEN_LDB_H
#define TOKEN_LDB_H

#include "TokenInstr.h"
#include <vector>

class TokenLdb : public TokenInstr {
public:
  TokenLdb(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 12) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
  std::string label;
};

#endif
