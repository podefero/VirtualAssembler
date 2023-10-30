#ifndef TOKEN_BNZ_H
#define TOKEN_BNZ_H

#include "TokenInstr.h"
#include <vector>

class TokenBnz : public TokenInstr {
public:
    TokenBnz(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 3) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override;

    std::string label;
};

#endif
