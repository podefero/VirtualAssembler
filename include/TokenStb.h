#ifndef TOKEN_STB_H
#define TOKEN_STB_H

#include "TokenInstr.h"
#include <vector>

class TokenStb : public TokenInstr {
public:
  TokenStb(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 11) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
