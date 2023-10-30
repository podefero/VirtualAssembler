#ifndef TOKEN_MOVI_H
#define TOKEN_MOVI_H

#include "TokenInstr.h"
#include <vector>

class TokenMovi : public TokenInstr {
public:
    TokenMovi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 31) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override;

    std::string label;
};

#endif
