#ifndef TOKEN_TRAP_H
#define TOKEN_TRAP_H

#include "TokenInstr.h"
#include <vector>

class TokenTrap : public TokenInstr {
public:
  TokenTrap(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 21) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
