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
    EXPECT_THROW(invalid.validate(memory), MemoryException);
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
    EXPECT_THROW(vm.fetch(), MemoryException);
}

TEST(VMTEST, Decode) {
    VirtualMachine vm;
    EXPECT_THROW(vm.decode(), MemoryException);
}

TEST(VMTEST, Execute) {
    VirtualMachine vm;
    EXPECT_NO_THROW(vm.execute());
}

TEST(VMTEST, FindTrap0) {
    VirtualMachine vm;
    EXPECT_THROW(vm.findTrap0(), MemoryException);
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
    Memory::Instruction instruction{1, 2, 3};
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
    vm.memory.code_seg_start = 13;
    vm.memory.code_seg_end = 1024;
    vm.setStackPointers(vm.memory.code_seg_end);
    //SL Points to next byte after Code Segment  (top) at runtime
    EXPECT_EQ(1028, vm.memory.registers.getRegister(Registers::SL));
    //SB Points next byte after size (base) at runtime
    EXPECT_EQ(2052, vm.memory.registers.getRegister(Registers::SB));
    EXPECT_EQ(2048, vm.memory.registers.getRegister(Registers::FP));
    EXPECT_EQ(2048, vm.memory.registers.getRegister(Registers::SP));
}

TEST(LOGICTEST, AndLogic) {
    Memory memory;
    memory.registers.setRegister(0, 1);
    memory.registers.setRegister(1, 1);
    memory.registers.setRegister(4, 66);
    OperationAND opTrue(OpcodeUtil::getOpcode(OpCode::AND), 0, 1);
    OperationAND opFalse(OpcodeUtil::getOpcode(OpCode::AND), 1, 4);

    opTrue.execute(memory); //r0 should be 1
    opFalse.execute(memory); //r1 should be 0

    EXPECT_EQ(1, memory.registers.getRegister(0));
    EXPECT_EQ(-1, memory.registers.getRegister(1));

}

TEST(LOGICTEST, OrLogic) {
    Memory memory;
    memory.registers.setRegister(0, 1);
    memory.registers.setRegister(1, 0);
    memory.registers.setRegister(2, 0);
    OperationOR opTrue(OpcodeUtil::getOpcode(OpCode::OR), 0, 1);
    OperationOR opFalse(OpcodeUtil::getOpcode(OpCode::OR), 1, 2);

    opTrue.execute(memory); //r0 should be 1
    opFalse.execute(memory); //r1 should be 0

    EXPECT_EQ(1, memory.registers.getRegister(0));
    EXPECT_EQ(-1, memory.registers.getRegister(1));

}

TEST(LOGICTEST, NotLogic) {
    Memory memory;
    memory.registers.setRegister(0, 1);
    memory.registers.setRegister(1, 0);
    memory.registers.setRegister(2, 0);
    OperationOR opTrue(OpcodeUtil::getOpcode(OpCode::OR), 0, 1);
    OperationOR opFalse(OpcodeUtil::getOpcode(OpCode::OR), 1, 2);

    opTrue.execute(memory); //r0 should be 1
    opFalse.execute(memory); //r1 should be 0

    EXPECT_EQ(1, memory.registers.getRegister(0));
    EXPECT_EQ(-1, memory.registers.getRegister(1));
}

TEST(SPTEST, PUSH) {
    VirtualMachine vm;
    //define data and code segs
    vm.memory.data_seg_end = 12;
    vm.memory.code_seg_start = 13;
    vm.memory.code_seg_end = 1024;
    vm.setStackPointers(vm.memory.code_seg_end);

    vm.memory.registers.setRegister(0, 10);
    //push the value of r0 onto stack
    OperationPUSH op(OpcodeUtil::getOpcode(OpCode::PUSH), 0, 0);
    op.execute(vm.memory);
    int expectValue = vm.memory.readInt(vm.memory.registers.getRegister(Registers::SP) + 4); //peek
    EXPECT_EQ(10, expectValue);
}

TEST(SPTEST, PUSH_OverFlow) {
    VirtualMachine vm;
    //define data and code segs
    vm.memory.data_seg_end = 12;
    vm.memory.code_seg_start = 13;
    vm.memory.code_seg_end = 1024;
    vm.setStackPointers(vm.memory.code_seg_end);

    vm.memory.registers.setRegister(0, 10);

    //set sp to sl
    int stack_limit = vm.memory.registers.getRegister(Registers::SL);
    vm.memory.registers.setRegister(Registers::SP, stack_limit);

    //push the value of r0 onto stack
    OperationPUSH op(OpcodeUtil::getOpcode(OpCode::PUSH), 0, 0);
    op.validate(vm.memory);
    op.execute(vm.memory); // we are now in overflow

    //typically I would validate first then execute, but in this case
    // I executed first to move SP above SL, then validated we are out of the stack
    EXPECT_THROW(op.validate(vm.memory), MemoryException);
}

TEST(SPTEST, POP) {
    VirtualMachine vm;
    //define data and code segs
    vm.memory.data_seg_end = 12;
    vm.memory.code_seg_start = 13;
    vm.memory.code_seg_end = 1024;
    vm.setStackPointers(vm.memory.code_seg_end);

    vm.memory.registers.setRegister(0, 10);

    //push the value of r0 onto stack
    OperationPUSH op(OpcodeUtil::getOpcode(OpCode::PUSH), 0, 0);
    op.execute(vm.memory);

    OperationPOP pop(OpcodeUtil::getOpcode(OpCode::POP), 1, 0); //load value in r1
    pop.execute(vm.memory);
    int expectValue = vm.memory.registers.getRegister(1);
    EXPECT_EQ(10, expectValue);
}

TEST(SPTEST, POP_UnderFlow) {
    VirtualMachine vm;
    //define data and code segs
    vm.memory.data_seg_end = 12;
    vm.memory.code_seg_start = 13;
    vm.memory.code_seg_end = 1024;
    vm.setStackPointers(vm.memory.code_seg_end);

    //pop value, sp should be at sb now
    OperationPOP op(OpcodeUtil::getOpcode(OpCode::POP), 0, 0);
    op.validate(vm.memory); //should be fine here
    op.execute(vm.memory);

    //typically I would validate first then execute, but in this case
    // I executed first to move SP at SB, then validated we are out of the stack
    EXPECT_THROW(op.validate(vm.memory), MemoryException);
}

TEST(FPTest, FirstActivationRec) {
    VirtualMachine vm;
    vm.memory.data_seg_end = 12;
    vm.memory.code_seg_start = 13;
    vm.memory.code_seg_end = 1024;
    vm.setStackPointers(vm.memory.code_seg_end);
    int fp = vm.memory.registers.getRegister(Registers::FP);
    int sp = vm.memory.registers.getRegister(Registers::SP);
    //they should point to same address
    EXPECT_EQ(fp, sp);
    //push return address, pfp, function param
    OperationPUSH ra(OpcodeUtil::getOpcode(OpCode::PUSH), 0, 0);
    OperationPUSH pfp(OpcodeUtil::getOpcode(OpCode::PUSH), 0, 0);
    OperationPUSH param(OpcodeUtil::getOpcode(OpCode::PUSH), 0, 0);
    //pushed 12 bytes on stack
    ra.execute(vm.memory);
    pfp.execute(vm.memory);
    param.execute(vm.memory);
    int sp_new = vm.memory.registers.getRegister(Registers::SP);
    int result = sp - sp_new;
    EXPECT_EQ(12, result);
}
