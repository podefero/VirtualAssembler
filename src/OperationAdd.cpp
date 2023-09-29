#include "OperationAdd.h"
#include "OperationException.h"
OperationAdd::OperationAdd(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

// no memory to validate
int OperationAdd::validate(Memory &memory) { return 1; }

int OperationAdd::execute(Memory &memory) {
  // op1 is destination op2 is source
  try {
    int rd = memory.registers.getRegister(operand1);
    int rs = memory.registers.getRegister(operand2);
    rd += rs;
    memory.registers.setRegister(operand1, rd);
  } catch (const std::out_of_range &ex) {
    return -1;
  }
  return 1;
}
