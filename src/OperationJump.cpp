#include "OperationJump.h"
#include "MemoryException.h"
OperationJump::OperationJump(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationJump::validate(Memory &memory) {
  // check if we are in of data_seg space
  if (operand1 <= memory.data_seg_end) {
    throw(MemoryException(
        "Jump is in data segment space, must be in code segment space"));
    return -1;
  } // Check if we are out of code_seg space
  else if (operand1 >= memory.code_seg_end) {
    throw(MemoryException("Jump is out of memory space"));
    return -1;
  }
  return 1;
}

int OperationJump::execute(Memory &memory) {
  memory.pc = operand1;
  return 1;
}
