#ifndef TOKEN_MOVE_H
#define TOKEN_MOVE_H

#include "TokenInstr.h"
#include <vector>

class TokenMove : public TokenInstr {
public:
  TokenMove(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 7) {}
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
};

#endif
