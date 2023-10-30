#ifndef TOKEN_BRZ_H
#define TOKEN_BRZ_H

#include "TokenInstr.h"
#include <vector>

class TokenBrz : public TokenInstr {
public:
    TokenBrz(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 6) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override;

    std::string label;
};

#endif
