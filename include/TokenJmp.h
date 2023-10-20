#ifndef TOKEN_JMP_H
#define TOKEN_JMP_H

#include "TokenInstr.h"
#include <vector>

class TokenJmp : public TokenInstr {
public:
  TokenJmp(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 1) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
  std::string label;
};

#endif
