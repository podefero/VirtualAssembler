#ifndef TOKEN_BYTE_H
#define TOKEN_BYTE_H

#include "Token.h"
#include <vector>

class TokenByte : public Token {
public:
  TokenByte(unsigned int offset, unsigned char value);
  void validate(unsigned int &offset) override;
  std::vector<unsigned char> getBytes() override;

private:
  unsigned char value;
};

#endif
