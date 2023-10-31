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
    bool inDataSeg = false;
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

    void validate(std::map<std::string, unsigned int> &symbol_table, unsigned int &data_seg) override {
        if (!label.empty()) {
            unsigned int label_offset = symbol_table.at(label);

            //If validation needs to be in data segment
            if (inDataSeg) {
                if (isInDataSeg(data_seg, label_offset)) {
                    op2 = label_offset;
                } else {
                    throw PassTwoException("Can't resolve label inDataSeg. label:" + label);
                }
            } else {
                //validation in code segment
                if (isInCodeSeg(data_seg, label_offset)) {
                    //jump is the only one to resolve label in operand 1
                    if (opCode == OpCode::JMP) {
                        op1 = label_offset;
                    } else {
                        op2 = label_offset;
                    }
                } else {
                    throw PassTwoException("Can't resolve label inCodeSeg. label:" + label);
                }
            }
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

#endif
