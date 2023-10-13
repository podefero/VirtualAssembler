#ifndef TOKEN_INSTRUCTION_H
#define TOKEN_INSTRUCTION_H

#include "Token.h"
#include <vector>

class TokenInstruction : public Token {
public:
  TokenInstruction(unsigned int offset, unsigned int opcode, int op1, int op2);
  void validate(unsigned int &offset);
  std::vector<unsigned char> getBytes();

private:
  unsigned int offset;
  unsigned int opcode;
  int op1;
  int op2;
};

#endif
