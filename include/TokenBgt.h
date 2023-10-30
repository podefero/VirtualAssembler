#ifndef TOKEN_BGT_H
#define TOKEN_BGT_H

#include "TokenInstr.h"
#include <vector>

class TokenBgt : public TokenInstr {
public:
    TokenBgt(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 4) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override;

    std::string label;
};

#endif
