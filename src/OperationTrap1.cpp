#include "OperationTrap1.h"
#include "TrapException.h"
#include <iostream>
OperationTrap1::OperationTrap1(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationTrap1::validate(Memory &memory) { return 1; }

int OperationTrap1::execute(Memory &memory) {
  // throw(TrapException("TRAP 1 INTERRUPT"));
  // print r3 to console
  int r3 = memory.registers.getRegister(3);
  std::cout << r3;
  return 1; // return 1 for trap 1
}
