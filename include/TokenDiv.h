#ifndef TOKEN_DIV_H
#define TOKEN_DIV_H

#include "TokenInstr.h"
#include <vector>

class TokenDiv : public TokenInstr {
public:
  TokenDiv(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 17) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
