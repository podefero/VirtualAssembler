#ifndef TOKEN_INSTR_H
#define TOKEN_INSTR_H

#include "Token.h"
#include <vector>

class TokenInstr : public Token {
public:
  virtual ~TokenInstr() = default;
  TokenInstr(unsigned int offset, unsigned int op1, unsigned int op2,
             unsigned int opcode) {
    this->offset = offset;
    this->op1 = op1;
    this->op2 = op2;
    this->opcode = opcode;
  }

  std::vector<unsigned char> getBytes() override {
    // Convert the opcode, op1, and op2 to bytes
    std::vector<unsigned char> bytes;

    // Extract the individual bytes of opcode
    bytes.push_back(static_cast<unsigned char>(opcode & 0xFF));
    bytes.push_back(static_cast<unsigned char>((opcode >> 8) & 0xFF));
    bytes.push_back(static_cast<unsigned char>((opcode >> 16) & 0xFF));
    bytes.push_back(static_cast<unsigned char>((opcode >> 24) & 0xFF));

    // Extract the individual bytes of op1
    bytes.push_back(static_cast<unsigned char>(op1 & 0xFF));
    bytes.push_back(static_cast<unsigned char>((op1 >> 8) & 0xFF));
    bytes.push_back(static_cast<unsigned char>((op1 >> 16) & 0xFF));
    bytes.push_back(static_cast<unsigned char>((op1 >> 24) & 0xFF));

    // Extract the individual bytes of op2
    bytes.push_back(static_cast<unsigned char>(op2 & 0xFF));
    bytes.push_back(static_cast<unsigned char>((op2 >> 8) & 0xFF));
    bytes.push_back(static_cast<unsigned char>((op2 >> 16) & 0xFF));
    bytes.push_back(static_cast<unsigned char>((op2 >> 24) & 0xFF));

    return bytes;
  }

private:
  unsigned int opcode;
  unsigned int op1;
  unsigned int op2;
};

#endif
