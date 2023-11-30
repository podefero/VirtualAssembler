#ifndef OPERATION_FACTORY_H
#define OPERATION_FACTORY_H

#include "Operations.h"
#include "Opcodes.h"

class OperationFactory {
public:
    static Operation *createOperation(int opcode, int op1, int op2) {
        auto opCodeEnum = static_cast<OpCode>(opcode);
        switch (opCodeEnum) {
            case OpCode::JMP:
                return new OperationJump(opcode, op1, op2);
            case OpCode::JMR:
                return new OperationJMR(opcode, op1, op2);
            case OpCode::BNZ:
                return new OperationBNZ(opcode, op1, op2);
            case OpCode::BGT:
                return new OperationBGT(opcode, op1, op2);
            case OpCode::BLT:
                return new OperationBLT(opcode, op1, op2);
            case OpCode::BRZ:
                return new OperationBRZ(opcode, op1, op2);
            case OpCode::CMP:
                return new OperationCMP(opcode, op1, op2);
            case OpCode::CMPI:
                return new OperationCMPI(opcode, op1, op2);
            case OpCode::MOV:
                return new OperationMove(opcode, op1, op2);
            case OpCode::MOVI:
                return new OperationMOVI(opcode, op1, op2);
            case OpCode::LDA:
                return new OperationLDA(opcode, op1, op2);
            case OpCode::STR:
                return new OperationSTR(opcode, op1, op2);
            case OpCode::STRI:
                return new OperationSTRI(opcode, op1, op2);
            case OpCode::LDR:
                return new OperationLDR(opcode, op1, op2);
            case OpCode::LDRI:
                return new OperationLDRI(opcode, op1, op2);
            case OpCode::STB:
                return new OperationSTB(opcode, op1, op2);
            case OpCode::STBI:
                return new OperationSTBI(opcode, op1, op2);
            case OpCode::LDB:
                return new OperationLDB(opcode, op1, op2);
            case OpCode::LDBI:
                return new OperationLDBI(opcode, op1, op2);
            case OpCode::ADD:
                return new OperationAdd(opcode, op1, op2);
            case OpCode::ADDI:
                return new OperationAddI(opcode, op1, op2);
            case OpCode::SUB:
                return new OperationSub(opcode, op1, op2);
            case OpCode::MUL:
                return new OperationMul(opcode, op1, op2);
            case OpCode::DIV:
                return new OperationDiv(opcode, op1, op2);
            case OpCode::AND:
                return new OperationAND(opcode, op1, op2);
            case OpCode::OR:
                return new OperationOR(opcode, op1, op2);
            case OpCode::NOT:
                return new OperationNOT(opcode, op1, op2);
            case OpCode::PUSH:
                return new OperationPUSH(opcode, op1, op2);
            case OpCode::POP:
                return new OperationPOP(opcode, op1, op2);
            case OpCode::PEEK:
                return new OperationPEEK(opcode, op1, op2);
            case OpCode::TRAP:
                switch (op1) {
                    case 0:
                        return new OperationTrap0(opcode, op1, op2);
                    case 1:
                        return new OperationTrap1(opcode, op1, op2);
                    case 2:
                        return new OperationTrap2(opcode, op1, op2);
                    case 3:
                        return new OperationTrap3(opcode, op1, op2);
                    case 4:
                        return new OperationTrap4(opcode, op1, op2);
                }
                break; // You might want to handle the default case
            default:
                // Return a default operation or throw an exception for unrecognized opcodes
                throw MemoryException("Invalid Instruction");
        }
        throw MemoryException("Invalid Instruction");
    }
};

#endif
