#include "OperationLDB.h"
#include "MemoryException.h"
OperationLDB::OperationLDB(int op1, int op2) {
  operand1 = op1;
  operand2 = op2;
}

int OperationLDB::validate(Memory &memory) {
  // check if we are out of data_seg space
  if (operand2 > memory.data_seg_end) {
    throw(MemoryException("LDB is past data segment"));
    return -1;
  } // Check if we are before data_seg
  else if (operand2 < memory.data_seg_start) {
    throw(MemoryException("LDB is before data segment"));
    return -1;
  }
  return 1;
}

int OperationLDB::execute(Memory &memory) {
  try {
    // byte directive
    memory.registers.setRegister(operand1, memory.readByte(operand2));
  } catch (const std::out_of_range &ex) {
    return -1;
  } catch (const MemoryException &ex) {
    return -1;
  }
  return 1;
}
