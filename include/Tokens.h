#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "PassTwoException.h"

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

    TokenInstr(unsigned int op1, unsigned int op2, unsigned int opcode) {
        this->op1 = op1;
        this->op2 = op2;
        this->opcode = opcode;
    }

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
    unsigned int opcode;
    unsigned int op1;
    unsigned int op2;
};

class TokenAdd : public TokenInstr {
public:
    TokenAdd(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 13) {}

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
    TokenDiv(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 17) {}

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
    TokenJmp(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 1) {}

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
    TokenJmr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 2) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &data_seg) override {

        if (isInDataSeg(data_seg, op1))
            throw PassTwoException("JMR label is in data seg. Should be in code seg.");

    }

    std::string label;
};

class TokenLdb : public TokenInstr {
public:
    TokenLdb(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 12) {}

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
    TokenLdr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 10) {}

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
    TokenMove(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 7) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenMul : public TokenInstr {
public:
    TokenMul(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 16) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenStb : public TokenInstr {
public:
    TokenStb(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 11) {}

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
    TokenStr(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 9) {}

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
    TokenSub(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 15) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenTrap : public TokenInstr {
public:
    TokenTrap(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 21) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {}
};

class TokenBgt : public TokenInstr {
public:
    TokenBgt(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 4) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {
        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BGT label not in code seg.");
    }

    std::string label;
};

class TokenBlt : public TokenInstr {
public:
    TokenBlt(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 5) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {

        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BLT label not in code seg.");
    }

    std::string label;
};

class TokenBnz : public TokenInstr {
public:
    TokenBnz(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 3) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {

        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BNZ label not in code seg.");
    }

    std::string label;
};

class TokenBrz : public TokenInstr {
public:
    TokenBrz(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 6) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override{

        unsigned int label_offset = symbol_table.at(label);
        if (isInCodeSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("BRZ label not in code seg.");
    }

    std::string label;
};

class TokenLda : public TokenInstr {
public:
    TokenLda(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 8) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override {

        unsigned int label_offset = symbol_table.at(label);
        if (isInDataSeg(limit, label_offset))
            op2 = label_offset;
        else
            throw PassTwoException("LDA label not in data seg.");
    }

    std::string label;
};

class TokenLdbi : public TokenInstr {
public:
    TokenLdbi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 25) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override{

        if (!isInDataSeg(limit, op2))
            throw PassTwoException("LDB Op2 address not in data seg.");
    }

    std::string label;
};

class TokenLdri : public TokenInstr {
public:
    TokenLdri(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 23) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override{
        if (!isInDataSeg(limit, op2))
            throw PassTwoException("LDR Op2 address not in data seg.");
    }

    std::string label;
};

class TokenMovi : public TokenInstr {
public:
    TokenMovi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 31) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override{}

    std::string label;
};

class TokenStbi : public TokenInstr {
public:
    TokenStbi(unsigned int op1, unsigned int op2) : TokenInstr(op1, op2, 24) {}

    void validate(std::map<std::string, unsigned int> &symbol_table,
                  unsigned int &limit) override{
        if (!isInDataSeg(limit, op2))
            throw PassTwoException("STB Op2 address not in data seg.");
    }

    std::string label;
};


#endif
