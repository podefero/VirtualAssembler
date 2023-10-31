#ifndef OPERATION_FACTORY_H
#define OPERATION_FACTORY_H

#include "Operations.h"

class OperationFactory {
public:
    static Operation *createOperation(int opcode, int op1, int op2) {
        switch (opcode) {
            case 1:
                return new OperationJump(opcode, op1, op2);
            case 2:
                return new OperationJMR(opcode, op1, op2);
            case 3:
                return new OperationBNZ(opcode, op1, op2);
            case 7:
                return new OperationMove(opcode, op1, op2);
            case 9:
                return new OperationSTR(opcode, op1, op2);
            case 10:
                return new OperationLDR(opcode, op1, op2);
            case 11:
                return new OperationSTB(opcode, op1, op2);
            case 12:
                return new OperationLDB(opcode, op1, op2);
            case 13:
                return new OperationAdd(opcode, op1, op2);
            case 15:
                return new OperationSub(opcode, op1, op2);
            case 16:
                return new OperationMul(opcode, op1, op2);
            case 17:
                return new OperationDiv(opcode, op1, op2);
            case 21:
                if (op1 == 0)
                    return new OperationTrap0(opcode, op1, op2);
                if (op1 == 1)
                    return new OperationTrap1(opcode, op1, op2);
                if (op1 == 2)
                    return new OperationTrap2(opcode, op1, op2);
                if (op1 == 3)
                    return new OperationTrap3(opcode, op1, op2);
                if (op1 == 4)
                    return new OperationTrap4(opcode, op1, op2);
            default:
                // Return a default operation or throw an exception for unrecognized
                // opcodes
                return nullptr;
        }
    }
};

#endif
