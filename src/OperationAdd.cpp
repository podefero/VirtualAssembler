#include "OperationAdd.h"

OperationAdd::OperationAdd(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationAdd::validate(Memory &memory) { return 0; }

int OperationAdd::execute(Registers &registers) { return 0; }
