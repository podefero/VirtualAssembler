#ifndef TOKEN_INT_H
#define TOKEN_INT_H

#include "Token.h"
#include <vector>

class TokenInt : public Token {
public:
  TokenInt(int value);
  void validate(std::map<std::string, unsigned int> &symbol_table,
                unsigned int &limit) override;
  std::vector<unsigned char> getBytes() override;

private:
  unsigned char value;
};

#endif
