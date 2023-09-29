#include "OperationTrap3.h"
#include "TrapException.h"
OperationTrap3::OperationTrap3(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationTrap3::validate(Memory &memory) { return 1; }

int OperationTrap3::execute(Memory &memory) {
  // throw(TrapException("TRAP INTERRUPT"));
  int r3 = memory.registers.getRegister(3);
  std::cout << char(r3);
  return 3; // return 3 for trap 3
}
