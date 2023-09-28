#ifndef OPERATION_FACTORY_H
#define OPERATION_FACTORY_H

#include "Operation.h"
#include "OperationAdd.h"

class OperationFactory {
public:
  static Operation *createOperation(int opcode, int op1, int op2) {
    switch (opcode) {
    case 13:
      return new OperationAdd(op1, op2);
    default:
      // Return a default operation or throw an exception for unrecognized
      // opcodes
      return nullptr;
    }
  }
};

#endif
