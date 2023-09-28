#include "VirtualMachine.h"
#include "MemoryException.h"
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
void VirtualMachine::initPc() {
  try {
    program_counter = memory.readInt(0);
  } catch (const MemoryException &ex) {
    std::cerr << ex.what();
  }
}

// fetch an instruction and set the instruction value and upate pc
void VirtualMachine::fetch() {
  try {
    current_instruction = memory.readInstruction(program_counter);
    program_counter += sizeof(Memory::Instruction);
  } catch (const MemoryException &ex) {
    std::cerr << ex.what();
  }
}

// getters
unsigned int VirtualMachine::getPc() { return program_counter; }

Memory::Instruction VirtualMachine::getInstruction() {
  return current_instruction;
}
