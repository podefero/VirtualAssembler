#ifndef TOKEN_MUL_H
#define TOKEN_MUL_H

#include "TokenInstr.h"
#include <vector>

class TokenMul : public TokenInstr {
public:
  TokenMul(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 16) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
