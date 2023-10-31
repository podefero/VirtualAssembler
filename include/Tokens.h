#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "PassTwoException.h"
#include "Opcodes.h"

class Token {
public:
    virtual ~Token() = default;

    virtual void validate(std::map<std::string, unsigned int> &symbol_table,
                          unsigned int &data_seg) {}

    virtual std::vector<unsigned char> getBytes() { return {}; };
    std::string label;
};

class TokenInstr : public Token {
public:
    ~TokenInstr() override = default;

    TokenInstr(unsigned int op1, unsigned int op2, OpCode opCode) : op1(op1), op2(op2), opCode(opCode) {}

    //if we are over data seg
    static bool isInCodeSeg(unsigned int &data_seg, unsigned int label_offset) {
        if (label_offset > data_seg) {
            return true;
        } else {
            return false;
        }
    }

    static bool isInDataSeg(unsigned int &data_seg, unsigned int label_offset) {
        if (label_offset < data_seg) {
            return true;
        } else {
            return false;
        }
    }

    std::vector<unsigned char> getBytes() override {
        std::vector<unsigned char> bytes;
        auto opcode = static_cast<unsigned int>(opCode);
        bytes.push_back(static_cast<unsigned char>(opcode & 0xFF));
        bytes.push_back(static_cast<unsigned char>((opcode >> 8) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((opcode >> 16) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((opcode >> 24) & 0xFF));

        bytes.push_back(static_cast<unsigned char>(op1 & 0xFF));
        bytes.push_back(static_cast<unsigned char>((op1 >> 8) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((op1 >> 16) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((op1 >> 24) & 0xFF));

        bytes.push_back(static_cast<unsigned char>(op2 & 0xFF));
        bytes.push_back(static_cast<unsigned char>((op2 >> 8) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((op2 >> 16) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((op2 >> 24) & 0xFF));

        return bytes;
    }

protected:
    OpCode opCode;
    unsigned int op1;
    unsigned int op2;
};

class TokenAdd : public TokenInstr {
public:
    TokenAdd(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::ADD) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenByte : public Token {
public:
    explicit TokenByte(unsigned char value) {
        this->value = value;
    }

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}

    std::vector<unsigned char> getBytes() override {
        std::vector<unsigned char> bytes;
        bytes.push_back(value);
        return bytes;
    }

private:
    unsigned char value;
};

class TokenDiv : public TokenInstr {
public:
    TokenDiv(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::DIV) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenInt : public Token {
public:
    explicit TokenInt(int value) {
        this->value = value;
    }

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}

    std::vector<unsigned char> getBytes() override {
        std::vector<unsigned char> bytes;
        bytes.push_back(static_cast<unsigned char>(value & 0xFF));
        bytes.push_back(static_cast<unsigned char>((value >> 8) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((value >> 16) & 0xFF));
        bytes.push_back(static_cast<unsigned char>((value >> 24) & 0xFF));
        return bytes;
    }

private:
    int value;
};

class TokenJmp : public TokenInstr {
public:
    TokenJmp(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::JMP) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &data_seg) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(data_seg, label_offset))
            op1 = label_offset;
        else
            throw PassTwoException("JMP label is in data seg. Should be in code seg.");
    }
};

class TokenJmr : public TokenInstr {
public:
    TokenJmr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::JMR) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &data_seg) override {

        if (isInDataSeg(data_seg, op1))
            throw PassTwoException("JMR label is in data seg. Should be in code seg.");

    }

};

class TokenLdb : public TokenInstr {
public:
    TokenLdb(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::LDB) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInDataSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("LDB label after data seg.");
    }
};

class TokenLdr : public TokenInstr {
public:
    TokenLdr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::LDR) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInDataSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("LDR label after data seg.");
    }
};

class TokenMove : public TokenInstr {
public:
    TokenMove(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::MOV) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenMul : public TokenInstr {
public:
    TokenMul(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::MUL) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenStb : public TokenInstr {
public:
    TokenStb(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::STB) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInDataSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("STB label not in data seg.");
    }
};

class TokenStr : public TokenInstr {
public:
    TokenStr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::STR) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInDataSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("StR label not in data seg.");
    }
};

class TokenSub : public TokenInstr {
public:
    TokenSub(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::SUB) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenTrap : public TokenInstr {
public:
    TokenTrap(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::TRAP) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenBgt : public TokenInstr {
public:
    TokenBgt(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::BGT) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BGT label not in code seg.");
    }
};

class TokenBlt : public TokenInstr {
public:
    TokenBlt(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::BLT) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {

        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BLT label not in code seg.");
    }
};

class TokenBnz : public TokenInstr {
public:
    TokenBnz(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::BNZ) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BNZ label not in code seg.");
    }
};

class TokenBrz : public TokenInstr {
public:
    TokenBrz(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::BRZ) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BRZ label not in code seg.");
    }

};

class TokenLda : public TokenInstr {
public:
    TokenLda(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::LDA) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {

        unsigned int label_offset = symbol_table.at(label);
        if (isInDataSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("LDA label not in data seg.");
    }

};

class TokenLdbi : public TokenInstr {
public:
    TokenLdbi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::LDBI) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {

        if (!isInDataSeg(limit, op2))
            throw PassTwoException("LDB Op2 address not in data seg.");
    }

};

class TokenLdri : public TokenInstr {
public:
    TokenLdri(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::LDRI) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        if (!isInDataSeg(limit, op2))
            throw PassTwoException("LDR Op2 address not in data seg.");
    }

};

class TokenMovi : public TokenInstr {
public:
    TokenMovi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::MOVI) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}

};

class TokenStbi : public TokenInstr {
public:
    TokenStbi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::STBI) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        if (!isInDataSeg(limit, op2))
            throw PassTwoException("STB Op2 address not in data seg.");
    }

};

class TokenCmpr : public TokenInstr {
public:
    TokenCmpr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, OpCode::STBI) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        if (!isInDataSeg(limit, op2))
            throw PassTwoException("STB Op2 address not in data seg.");
    }

};


#endif
