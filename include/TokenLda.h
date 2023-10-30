#ifndef TOKEN_LDA_H
#define TOKEN_LDA_H

#include "TokenInstr.h"
#include <vector>

class TokenLda : public TokenInstr {
public:
    TokenLda(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 8) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override;

    std::string label;
};

#endif
