#ifndef OPERATION_FACTORY_H
#define OPERATION_FACTORY_H

#include "Operation.h"
#include "OperationAdd.h"
#include "OperationDiv.h"
#include "OperationJump.h"
#include "OperationLDB.h"
#include "OperationLDR.h"
#include "OperationMove.h"
#include "OperationMul.h"
#include "OperationSub.h"
#include "OperationTrap0.h"
#include "OperationTrap1.h"
#include "OperationTrap3.h"

class OperationFactory {
public:
  static Operation *createOperation(int opcode, int op1, int op2) {
    switch (opcode) {
    case 1:
      return new OperationJump(op1, op2);
    case 7:
      return new OperationMove(op1, op2);
    case 10:
      return new OperationLDR(op1, op2);
    case 12:
      return new OperationLDB(op1, op2);
    case 13:
      return new OperationAdd(op1, op2);
    case 15:
      return new OperationSub(op1, op2);
    case 16:
      return new OperationMul(op1, op2);
    case 17:
      return new OperationDiv(op1, op2);
    case 22:
      if (op1 == 0)
        return new OperationTrap0(op1, op2);
      if (op1 == 1)
        return new OperationTrap1(op1, op2);
      if (op1 == 3)
        return new OperationTrap3(op1, op2);
    default:
      // Return a default operation or throw an exception for unrecognized
      // opcodes
      return nullptr;
    }
  }
};

#endif
