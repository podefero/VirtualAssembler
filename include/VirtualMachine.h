#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include "Memory.h"
#include "Operations.h"
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

    void fetch();

    void decode();

    void execute();

    unsigned int findTrap0();

    void setStackPointers(unsigned int limit);

    Memory::Instruction getInstruction();

private:
    Memory::Instruction current_instruction{};
    Operation *operation{};
    OperationFactory operationFactory;
};

#endif
