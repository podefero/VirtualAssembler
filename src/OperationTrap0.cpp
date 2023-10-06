#include "OperationTrap0.h"
#include "TrapException.h"
OperationTrap0::OperationTrap0(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationTrap0::validate(Memory &memory) { return 1; }

int OperationTrap0::execute(Memory &memory) {
  throw(TrapException("TRAP INTERRUPT"));
  return 0; // return 0 for trap 0
}
