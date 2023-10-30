#ifndef TOKEN_LDRI_H
#define TOKEN_LDRI_H

#include "TokenInstr.h"
#include <vector>

class TokenLdri : public TokenInstr {
public:
  TokenLdri(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 23) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
