#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "Memory.h"
#include "Operation.h"
#include "OperationFactory.h"
#include "Registers.h"
#include <queue>
#include <string>

class VirtualMachine {
public:
  VirtualMachine();
  ~VirtualMachine();

  Memory memory;
  int loadMemory(const std::string &filePath);
  void initPc();
  void fetch();
  void decode();
  std::queue<Operation *> getOperationQueue();
  Memory::Instruction getInstruction();
  unsigned int getPc();

private:
  unsigned int program_counter = 0;
  Memory::Instruction current_instruction;
  Registers registers;
  OperationFactory operationFactory;
  std::queue<Operation *> operationQueue;
};

#endif
