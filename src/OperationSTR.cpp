#include "OperationSTR.h"
#include "MemoryException.h"
OperationSTR::OperationSTR(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationSTR::validate(Memory &memory) {
  // check if we are out of data_seg space
  // not counting the zero so minus 1
  if ((operand2 + sizeof(int)) - 1 > memory.data_seg_end) {
    throw(MemoryException("STR is past data segment"));
    return -1;
  } // Check if we are before data_seg
  else if (operand2 < memory.data_seg_start) {
    throw(MemoryException("STR is before data segment"));
    return -1;
  }
  return 1;
}

int OperationSTR::execute(Memory &memory) {
  try {
    // get regValue
    unsigned int rs = memory.registers.getRegister(operand1);
    unsigned int offset = operand2;
    // write rs to label
    memory.writeInt(offset, rs);
  } catch (const std::out_of_range &ex) {
    return -1;
  } catch (const MemoryException &ex) {
    return -1;
  }

  return 1;
}
