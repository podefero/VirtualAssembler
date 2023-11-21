#include "MemoryException.h"
#include "TrapException.h"
#include "VirtualMachine.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(OPTEST, MOV) {
    Memory memory;

    //set pc
    memory.registers.setRegister(Registers::PC, 25);
    //move pc to R1
    OperationMove move(OpcodeUtil::getOpcode(OpCode::MOV), 1, 16);
    move.execute(memory);

    //R1 should have 25 in it.
    int result = memory.registers.getRegister(1);
    EXPECT_EQ(25, result);

    //Shouldn't be able to move anything into PC
    OperationMove invalid(OpcodeUtil::getOpcode(OpCode::MOV), 16, 1);
    EXPECT_THROW(invalid.execute(memory), MemoryException);
}

TEST(VMTEST, LoadFile) {
    std::string filePath = "we.bin";
    std::string invalidFilePath = "idk.bin";
    VirtualMachine vm;
    EXPECT_NO_THROW(vm.loadMemory(filePath));
    EXPECT_THROW(vm.loadMemory(invalidFilePath), MemoryException);
}

TEST(VMTEST, InitPC) {
    VirtualMachine vm;
    EXPECT_NO_THROW(vm.initPc());
}

//All will throw errors, but the throw is caught before
//it can be handled here.
//Will fix this in future projects
TEST(VMTEST, Fetch) {
    VirtualMachine vm;
    //can't fetch with no data
    EXPECT_NO_THROW(vm.fetch());
}

TEST(VMTEST, Decode) {
    VirtualMachine vm;
    EXPECT_NO_THROW(vm.decode());
}

TEST(VMTEST, Execute) {
    VirtualMachine vm;
    EXPECT_NO_THROW(vm.execute());
}

TEST(VMTEST, FindTrap0) {
    VirtualMachine vm;
    EXPECT_NO_THROW(vm.findTrap0());
}

TEST(MEMTEST, LoadFile) {
    Memory memory;
    std::string filePath = "we.bin";
    std::string invalidFilePath = "idk.bin";
    EXPECT_NO_THROW(memory.loadFromFile(filePath));
    EXPECT_THROW(memory.loadFromFile(invalidFilePath), MemoryException);
}

TEST(MEMTEST, ReadWriteINT) {
    Memory memory;
    memory.writeInt(0, 0x08);
    int result = memory.readInt(0);
    EXPECT_EQ(8, result);
    EXPECT_THROW(memory.readInt(Memory::size), MemoryException);
    EXPECT_THROW(memory.writeInt(Memory::size, 0x01), MemoryException);
}

TEST(MEMTEST, ReadWriteByte) {
    Memory memory;
    memory.writeByte(0, 0x57);
    int result = memory.readByte(0);
    EXPECT_EQ('W', result);
    EXPECT_THROW(memory.readByte(Memory::size), MemoryException);
    EXPECT_THROW(memory.writeByte(Memory::size, 0x01), MemoryException);
}

TEST(MEMTEST, ReadWriteInstruction) {
    Memory memory;
    Memory::Instruction instruction {1, 2, 3};
    memory.writeInstruction(0, instruction);
    Memory::Instruction result = memory.readInstruction(0);
    EXPECT_EQ(instruction.opcode, result.opcode);
    EXPECT_EQ(instruction.operand1, result.operand1);
    EXPECT_EQ(instruction.operand2, result.operand2);
    EXPECT_THROW(memory.readInstruction(Memory::size), MemoryException);
    EXPECT_THROW(memory.writeInstruction(Memory::size, instruction), MemoryException);
}

TEST(STACK_REG_TEST, CheckStackPointers) {
    VirtualMachine vm;
    //define data and code segs
    vm.memory.data_seg_end = 12;
    vm.memory.data_seg_start = 13;
    vm.memory.data_seg_end = 1024;
    vm.setStackPointers(vm.memory.data_seg_end);
    //SL Points to next byte after Code Segment  (top) at runtime
    EXPECT_EQ(1028, vm.memory.registers.getRegister(Registers::SL));
    //SB Points next byte after size (base) at runtime
    EXPECT_EQ(2052, vm.memory.registers.getRegister(Registers::SB));
    EXPECT_EQ(2052, vm.memory.registers.getRegister(Registers::FP));
    EXPECT_EQ(2048, vm.memory.registers.getRegister(Registers::SP));
}
