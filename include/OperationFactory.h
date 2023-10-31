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
            case OpCode::MOV:
                return new OperationMove(opcode, op1, op2);
            case OpCode::STR:
                return new OperationSTR(opcode, op1, op2);
            case OpCode::LDR:
                return new OperationLDR(opcode, op1, op2);
            case OpCode::STB:
                return new OperationSTB(opcode, op1, op2);
            case OpCode::LDB:
                return new OperationLDB(opcode, op1, op2);
            case OpCode::ADD:
                return new OperationAdd(opcode, op1, op2);
            case OpCode::SUB:
                return new OperationSub(opcode, op1, op2);
            case OpCode::MUL:
                return new OperationMul(opcode, op1, op2);
            case OpCode::DIV:
                return new OperationDiv(opcode, op1, op2);
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
                return nullptr;
        }
        return nullptr;
    }
};

#endif
