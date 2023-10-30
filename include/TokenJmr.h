#ifndef TOKEN_JMR_H
#define TOKEN_JMR_H

#include "TokenInstr.h"
#include <vector>

class TokenJmr : public TokenInstr {
public:
  TokenJmr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 2) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
  std::string label;
};

#endif
