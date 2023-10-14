#ifndef TOKEN_BYTE_H
#define TOKEN_BYTE_H

#include "Token.h"
#include <vector>

class TokenInstr : public Token {
public:
  TokenInstr(unsigned int offset, unsigned char value);
  void validate(unsigned int &offset) override;
  std::vector<unsigned char> getBytes() override;

private:
  unsigned int op1;
  unsigned int op2;
};

#endif
