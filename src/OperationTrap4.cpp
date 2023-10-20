#include "OperationTrap4.h"
#include "MemoryException.h"
#include <iostream>
OperationTrap4::OperationTrap4(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationTrap4::validate(Memory &memory) { return 1; }

int OperationTrap4::execute(Memory &memory) {
  int value;
  std::cin >> value;
  if (std::cin.fail()) {
    throw MemoryException("Trap4 Invalid input");
  } else
    memory.registers.setRegister(3, value);
  return 4; // return 4 for trap 4
}
