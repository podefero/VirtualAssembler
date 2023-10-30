#ifndef TOKEN_LDBI_H
#define TOKEN_LDBI_H

#include "TokenInstr.h"
#include <vector>

class TokenLdbi : public TokenInstr {
public:
  TokenLdbi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 12) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
