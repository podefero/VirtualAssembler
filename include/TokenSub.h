#ifndef TOKEN_SUB_H
#define TOKEN_SUB_H

#include "TokenInstr.h"
#include <vector>

class TokenSub : public TokenInstr {
public:
  TokenSub(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 15) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
