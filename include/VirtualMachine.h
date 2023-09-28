#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "Memory.h"
#include "OperationFactory.h"
#include "Registers.h"
#include <string>

class VirtualMachine {
public:
  VirtualMachine();
  ~VirtualMachine();

  Memory memory;
  int loadMemory(const std::string &filePath);
  void initPc();
  void fetch();
  Memory::Instruction getInstruction();
  unsigned int getPc();

private:
  unsigned int program_counter = 0;
  Memory::Instruction current_instruction;
  Registers registers;
};

#endif
