#ifndef TOKEN_ADD_H
#define TOKEN_ADD_H

#include "TokenInstr.h"
#include <vector>

class TokenAdd : public TokenInstr {
public:
  TokenAdd(unsigned int offset, unsigned int op1, unsigned int op2)
      : TokenInstr(offset, op1, op2, 13) {}
  void validate(std::map<std::string, unsigned int> &symbol_table) override;

private:
  unsigned int opcode;
  unsigned int op1;
  unsigned int op2;
};

#endif
