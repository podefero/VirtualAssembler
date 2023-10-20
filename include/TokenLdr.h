#ifndef TOKEN_LDR_H
#define TOKEN_LDR_H

#include "TokenInstr.h"
#include <vector>

class TokenLdr : public TokenInstr {
public:
  TokenLdr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 10) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
