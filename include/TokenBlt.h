#ifndef TOKEN_BLT_H
#define TOKEN_BLT_H

#include "TokenInstr.h"
#include <vector>

class TokenBlt : public TokenInstr {
public:
    TokenBlt(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 5) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override;

    std::string label;
};

#endif
