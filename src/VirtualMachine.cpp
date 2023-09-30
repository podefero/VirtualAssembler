#include "VirtualMachine.h"
#include "MemoryException.h"
#include "TrapException.h"
#include <iostream>

// driver object that will hold everything needed to work with driver(main)
VirtualMachine::VirtualMachine() {}

VirtualMachine::~VirtualMachine() {}

// driver function to load bin file into memory object
int VirtualMachine::loadMemory(const std::string &filePath) {
  // try to read, if fail memory will throw custom exception which will be
  // handled here.
  try {
    memory.loadFromFile(filePath);
    // std::cerr << filePath;
  } catch (const MemoryException &ex) {
    std::cerr << ex.what();
    return -1;
  }
  return 1;
}

// set the program counter
// we can calculate the data_segment space
void VirtualMachine::initPc() {
  try {
    memory.pc = memory.readInt(0);
    // - 4 from int size then -1 becuaes we are starting byte
    memory.data_seg_end = memory.data_seg_start + (memory.pc - sizeof(int) - 1);
    memory.code_seg_start = memory.data_seg_end + 1; // next byte over
  } catch (const MemoryException &ex) {
    std::cerr << ex.what();
  }
}

// fetch an instruction and set the instruction value and upate pc
int VirtualMachine::fetch() {
  try {
    current_instruction = memory.readInstruction(memory.pc);
    memory.pc += sizeof(Memory::Instruction);
  } catch (const MemoryException &ex) {
    std::cerr << ex.what();
    return -1;
  }
  return 1;
}

// decode instruction to get operation, then validate it
int VirtualMachine::decode() {
  Operation *operation;
  operation = operationFactory.createOperation(current_instruction.opcode,
                                               current_instruction.operand1,
                                               current_instruction.operand2);
  int result = 0;
  if (operation) {
    try {
      result = operation->validate(memory);
      if (result == -1) {
        std::cerr << "Decode Failed" << std::endl;
        return -1;
      }
    } catch (const MemoryException &ex) {
      std::cerr << ex.what();
      return -1;
    }
    operationQueue.push(operation);
  } else { // operation is null
    std::cerr << "Null operation at offset "
              << memory.pc - sizeof(Memory::Instruction)
              << " Opcode: " << current_instruction.opcode
              << " op1: " << current_instruction.operand1
              << " op2: " << current_instruction.operand2 << std::endl;
    return -1;
  }
  delete operation;
  return 1;
}

// execute all operations, catch trap execeptions and handle those
int VirtualMachine::execute() {
  int result = 0;
  Operation *operation;
  while (!operationQueue.empty()) {

    try {

      operation = operationQueue.front();

      if (operation) {
        result = operation->execute(memory);

      } else {
        std::cerr << "Operation failed " << std::endl;
      }
      if (result == -1) {
        return -1;
      }
      std::cout << "check1" << std::endl;
      operationQueue.pop();

    } catch (const TrapException &ex) {
      // result from trap 0
      // exit gracefully
      std::cerr << "Successful exit" << std::endl;
      return 1;
    }
  }
  return 0;
}

// find trap 0 so we can defince the code_segment space
unsigned int VirtualMachine::findTrap0() {
  unsigned int offset = memory.pc;
  Memory::Instruction trap0 = {21, 0, 0};
  Memory::Instruction instruct;
  // loop until we get trap 0. return 1 if there is no trap.
  while (true) {
    try {
      instruct = memory.readInstruction(offset);
    } catch (const MemoryException &ex) {
      std::cerr << "No trap0" << ex.what();
      return 1;
    }
    offset += sizeof(Memory::Instruction);
    if (trap0.opcode == instruct.opcode &&
        trap0.operand1 == instruct.operand1) {
      memory.code_seg_end = offset;
      return offset;
    }
  }
}

// getters

Memory::Instruction VirtualMachine::getInstruction() {
  return current_instruction;
}
