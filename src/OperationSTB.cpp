#include "OperationSTB.h"
#include "MemoryException.h"
OperationSTB::OperationSTB(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationSTB::validate(Memory &memory) {
  // check if we are out of data_seg space
  // not counting the zero so minus 1
  if ((operand2 + sizeof(int)) - 1 > memory.data_seg_end) {
    throw(MemoryException("STB is past data segment"));
    return -1;
  } // Check if we are before data_seg
  else if (operand2 < memory.data_seg_start) {
    throw(MemoryException("STB is before data segment"));
    return -1;
  }
  return 1;
}

int OperationSTB::execute(Memory &memory) {
  try {
    // int directive
    memory.registers.setRegister(operand1, memory.readInt(operand2));
  } catch (const std::out_of_range &ex) {
    return -1;
  } catch (const MemoryException &ex) {
    return -1;
  }

  return 1;
}
