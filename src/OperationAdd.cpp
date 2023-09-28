#include "OperationAdd.h"
#include "OperationException.h"
OperationAdd::OperationAdd(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationAdd::validate(Memory &memory) { return 0; }

int OperationAdd::execute(Registers &registers) {
  try {
    int rd = registers.getRegister(operand1);
    int rs = registers.getRegister(operand2);
    rd += rs;
    registers.setRegister(operand1, rd);
  } catch (const std::out_of_range &ex) {
    return -1;
  }
  return 1;
}
