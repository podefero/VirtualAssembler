#include "VirtualMachine.h"
#include "MemoryException.h"
#include "TrapException.h"
#include <iostream>

// driver object that will hold everything needed to work with driver(main)
VirtualMachine::VirtualMachine() = default;

VirtualMachine::~VirtualMachine() = default;

// driver function to load bin file into memory object
int VirtualMachine::loadMemory(const std::string &filePath) {
    // try to read, if fail memory will throw custom exception which will be
    // handled here.
    try {
        memory.loadFromFile(filePath);
    } catch (const MemoryException &ex) {
        throw MemoryException(ex.what());
    }
    return 1;
}

// set the program counter
// we can calculate the data_segment space
void VirtualMachine::initPc() {
    try {
        memory.registers.setRegister(Registers::PC, memory.readInt(0));
        int pc = memory.registers.getRegister(Registers::PC);
        // - 4 from int size then -1 becuaes we are starting byte
        memory.data_seg_end = memory.data_seg_start + (pc - sizeof(int) - 1);
        memory.code_seg_start = memory.data_seg_end + 1; // next byte over
    } catch (const MemoryException &ex) {
        std::cerr << ex.what();
    }
}

// fetch an instruction and set the instruction value and upate pc
int VirtualMachine::fetch() {
    try {
        int pc = memory.registers.getRegister(Registers::PC);
        if (pc < memory.code_seg_start) {
            throw MemoryException("PC is in Data Segment");
        }

        current_instruction = memory.readInstruction(pc);
        memory.registers.setRegister(Registers::PC, pc + sizeof(Memory::Instruction));
        //update pc
        pc = memory.registers.getRegister(Registers::PC);
        if (pc > memory.code_seg_end) {
            throw MemoryException("PC is after Code Segment");
        }
    } catch (const MemoryException &ex) {
        memory.registers.dumpRegisters();
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 1;
}

// decode instruction to get operation, then validate it
int VirtualMachine::decode() {
    int pc = memory.registers.getRegister(Registers::PC);

    operation = OperationFactory::createOperation(current_instruction.opcode,
                                                  current_instruction.operand1,
                                                  current_instruction.operand2);
    if (operation) {
        try {
            operation->validate(memory);
        } catch (const MemoryException &ex) {
            memory.registers.dumpRegisters();
            std::cerr << "Code Seg Start: " << memory.code_seg_start << std::endl;
            std::cerr << "Code Seg End " << memory.code_seg_end << std::endl;
            std::cerr << std::endl;
            std::cerr << "Data Seg Start " << memory.data_seg_start << std::endl;
            std::cerr << "Data Seg End " << memory.data_seg_end << std::endl;
            std::cerr << "Opcode: " << operation->operationAsString();
            std::cerr << ex.what() << std::endl;
            return -1;
        }
    } else { // operation is null
        std::cerr << "Null operation at offset "
                  << pc - sizeof(Memory::Instruction)
                  << " Opcode: " << current_instruction.opcode
                  << " op1: " << current_instruction.operand1
                  << " op2: " << current_instruction.operand2 << std::endl;
        return -1;
    }
    return 1;
}

// execute all operations, catch trap execeptions and handle those
int VirtualMachine::execute() {
    try {
        if (operation) {
            operation->execute(memory);

        } else {
            std::cerr << "Operation failed " << std::endl;
        }
        delete operation;
    } catch (const TrapException &ex) {
        // result from trap 0
        // exit gracefully
        return 2;
    } catch (const MemoryException &ex) {
        memory.registers.dumpRegisters();
        std::cerr << "Code Seg Start: " << memory.code_seg_start << std::endl;
        std::cerr << "Code Seg End " << memory.code_seg_end << std::endl;
        std::cerr << std::endl;
        std::cerr << "Data Seg Start " << memory.data_seg_start << std::endl;
        std::cerr << "Data Seg End " << memory.data_seg_end << std::endl;
        std::cerr << "Opcode: " << operation->operationAsString();
        std::cerr << ex.what();
    }

    return 1;
}

// find trap 0 so we can define the code_segment space
unsigned int VirtualMachine::findTrap0() {
    int pc = memory.registers.getRegister(Registers::PC);

    unsigned int offset = pc;
    Memory::Instruction trap0 = {21, 0, 0};
    Memory::Instruction instruct{};
    // loop until we get trap 0. return 1 if there is no trap.
    while (true) {
        try {
            instruct = memory.readInstruction(offset);
        } catch (const MemoryException &ex) {
            std::cerr << "No trap0 -> " << ex.what();
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
