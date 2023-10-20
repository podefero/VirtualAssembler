#include "OperationTrap2.h"
#include "MemoryException.h"
OperationTrap2::OperationTrap2(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationTrap2::validate(Memory &memory) { return 1; }

int OperationTrap2::execute(Memory &memory) {
  unsigned char value;
  std::cin >> value;
  if (std::cin.fail()) {
    throw MemoryException("Trap2 Invalid input");
  } else
    memory.registers.setRegister(3, value);
  return 2; // return 2 for trap 2
}
