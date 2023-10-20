#ifndef TOKEN_STR_H
#define TOKEN_STR_H

#include "PassTwoException.h"
#include "TokenInstr.h"
#include <vector>

class TokenStr : public TokenInstr {
public:
  TokenStr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 9) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;

  std::string label;
};

#endif
