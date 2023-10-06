#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "Memory.h"
#include "Operation.h"
#include "OperationFactory.h"
#include <queue>
#include <string>

class VirtualMachine {
public:
  VirtualMachine();
  ~VirtualMachine();

  Memory memory;
  int loadMemory(const std::string &filePath);
  void initPc();
  int fetch();
  int decode();
  int execute();
  unsigned int findTrap0();
  Memory::Instruction getInstruction();

private:
  Memory::Instruction current_instruction;
  Operation *operation;
  OperationFactory operationFactory;
};

#endif
