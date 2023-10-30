#ifndef TOKEN_STBI_H
#define TOKEN_STBI_H

#include "TokenInstr.h"
#include <vector>

class TokenStbi : public TokenInstr {
public:
  TokenStbi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 24) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
