#include "MemoryException.h"
#include "VirtualMachine.h"
#include <gtest/gtest.h>
#include <string>

// Define a fixture for your tests if needed.
class VirtualMachineTest : public ::testing::Test {
protected:
  VirtualMachine vm;
  VirtualMachineTest() : vm() {}
};

// Load File Tests
TEST_F(VirtualMachineTest, LoadMemoryValid) {
  // Test case for loading memory with a valid file.
  std::string filePath = "we.bin";
  int result = vm.loadMemory(filePath);

  EXPECT_EQ(result, 1);
}

TEST_F(VirtualMachineTest, LoadMemoryInvalid) {
  // Test case for loading memory with an invalid file.
  std::string filePath = "invalid_test_file.bin";
  int result = vm.loadMemory(filePath);

  EXPECT_EQ(result, -1);
}

// Read Memory Tests
TEST_F(VirtualMachineTest, ReadValidPC) {
  // Test case to see if we can read bytes from we.bin file
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);
  int expect = 6;                    // pc value
  int result = vm.memory.readInt(0); // read first 4 bytes of memory
  EXPECT_EQ(result, expect);
}

TEST_F(VirtualMachineTest, ReadPastSize) {
  // Test exception when we try to read past main memory siz
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);
  int offset = 100; // beyond size of we.bin
  EXPECT_THROW(vm.memory.readInt(offset), MemoryException);
  // int result = vm.memory.read()
}

TEST_F(VirtualMachineTest, ReadByteValid) {
  // Test case to read a byte from valid memory
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  unsigned char expect = 0x57;                     // 'W' Expected byte value
  unsigned char result = vm.memory.readByte(0x04); // Read byte from memory
  EXPECT_EQ(result, expect);
}

TEST_F(VirtualMachineTest, ReadInstructionValid) {
  // Test case to read an instruction from valid memory
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  // Expected instruction values
  Memory::Instruction expect = {0x01, 0x12, 0x00};
  // Read instruction from memory
  Memory::Instruction result = vm.memory.readInstruction(6);
  EXPECT_EQ(result.opcode, expect.opcode);
  EXPECT_EQ(result.operand1, expect.operand1);
  EXPECT_EQ(result.operand2, expect.operand2);
}

TEST_F(VirtualMachineTest, InitPC) {
  // Test if we can read program counter
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.initPc();
  unsigned int expect = 0x06;
  EXPECT_EQ(expect, vm.memory.pc);
}

TEST_F(VirtualMachineTest, DataSegment) {
  // Test if data segment range is right for we.bin
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.initPc();
  // expects. 2 bytes range
  unsigned int seg_start = 4;
  unsigned int seg_end = 5;
  EXPECT_EQ(seg_start, vm.memory.data_seg_start);
  EXPECT_EQ(seg_end, vm.memory.data_seg_end);
}

TEST_F(VirtualMachineTest, CodeSegment) {
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.initPc();

  unsigned int seg_start = 7;
  unsigned int seg_end = 78; // file size 78 bytes.
  vm.findTrap0();
  EXPECT_EQ(seg_end, vm.memory.code_seg_end);
}

TEST_F(VirtualMachineTest, FetchAndUpdate) {
  // Test if we can fetch, validate instruction and validate updated pc
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.initPc();

  // Expected instruction values and updated pc value
  Memory::Instruction expect = {0x01, 0x12, 0x00};
  unsigned int expect_pc = 18;

  // set instruction and updated pc
  vm.fetch();

  EXPECT_EQ(vm.getInstruction().opcode, expect.opcode);
  EXPECT_EQ(vm.getInstruction().operand1, expect.operand1);
  EXPECT_EQ(vm.getInstruction().operand2, expect.operand2);
  EXPECT_EQ(vm.memory.pc, expect_pc);
}

TEST_F(VirtualMachineTest, FetchNDecode) {
  // Test the decode, see if we can get the first instruction which is move in
  // we.bin
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.initPc();

  // set instruction and updated pc
  vm.fetch();

  vm.decode();

  EXPECT_FALSE(vm.getOperationQueue().empty());
}

// Opcode tests
TEST_F(VirtualMachineTest, ValidateAddOpCode) {
  // Test the add opcode
  Memory memory;
  memory.registers.setRegister(0x01, 10);
  memory.registers.setRegister(0x02, 10);

  // reg 0x01 should be 20 now
  OperationAdd operation(0x01, 0x02);
  int result = operation.execute(memory);
  int reg_expect = 20;
  EXPECT_EQ(result, 1);
  EXPECT_EQ(reg_expect, memory.registers.getRegister(0x01));
}

TEST_F(VirtualMachineTest, ValidateSubOpCode) {
  // Test the sub opcode
  Memory memory;
  memory.registers.setRegister(0x01, 5);
  memory.registers.setRegister(0x02, 10);

  // reg 0x01 should be -5 now
  OperationSub operation(0x01, 0x02);
  int result = operation.execute(memory);
  int reg_expect = -5;
  EXPECT_EQ(result, 1);
  EXPECT_EQ(reg_expect, memory.registers.getRegister(0x01));
}

TEST_F(VirtualMachineTest, ValidateMulOpCode) {
  // Test the mul opcode
  Memory memory;
  memory.registers.setRegister(0x01, 5);
  memory.registers.setRegister(0x02, 5);

  // reg 0x01 should be 25 now
  OperationMul operation(0x01, 0x02);
  int result = operation.execute(memory);
  int reg_expect = 25;
  EXPECT_EQ(result, 1);
  EXPECT_EQ(reg_expect, memory.registers.getRegister(0x01));
}

TEST_F(VirtualMachineTest, ValidateDivOpCode) {
  // Test the div opcode
  Memory memory;
  memory.registers.setRegister(0x01, 5);
  memory.registers.setRegister(0x02, 2);

  // reg 0x01 should be 1 now
  OperationDiv operation(0x01, 0x02);
  int result = operation.execute(memory);
  int reg_expect = 2;
  EXPECT_EQ(result, 1);
  EXPECT_EQ(reg_expect, memory.registers.getRegister(0x01));
}

TEST_F(VirtualMachineTest, InvalidRegisterRange) {
  // Test opcode using register outside of range
  Memory memory;
  memory.registers.setRegister(0x01, 10);
  memory.registers.setRegister(0x02, 10);

  OperationAdd operation(17, 16);
  EXPECT_EQ(operation.execute(memory), -1);
}

TEST_F(VirtualMachineTest, ValidRegisterRange) {
  // Test opcode using register inside of range
  Memory memory;
  memory.registers.setRegister(0x01, 10);
  memory.registers.setRegister(0x02, 10);

  OperationAdd operation(15, 14);
  EXPECT_EQ(operation.execute(memory), 1);
}

TEST_F(VirtualMachineTest, ValidateMoveOpCode) {
  // Test the move opcode
  Memory memory;
  memory.registers.setRegister(0x01, 10);

  // reg 0x02 should be 10 now
  OperationMove operation(0x02, 0x01);
  int result = operation.execute(memory);
  int reg_expect = 10;
  EXPECT_EQ(result, 1);
  EXPECT_EQ(reg_expect, memory.registers.getRegister(0x02));
}

TEST_F(VirtualMachineTest, ValidateJumpOpCode) {
  // Test the jump opcode
  Memory memory;
  memory.data_seg_end = 0x02;
  memory.code_seg_end = 0x13;

  OperationJump operation(0x12, 0x00);
  // pc should be 0x12
  int decode = operation.validate(memory);
  EXPECT_EQ(decode, 1);
  int result = operation.execute(memory);
  int pc_expect = 0x12;
  EXPECT_EQ(result, 1);
  EXPECT_EQ(pc_expect, memory.pc);
}

TEST_F(VirtualMachineTest, InvValidateJumpOpCode) {
  // Test the jump opcode going into data seg
  Memory memory;
  memory.data_seg_end = 0x12;

  OperationJump operation(0x12, 0x00);
  EXPECT_THROW(operation.validate(memory), MemoryException);
}

TEST_F(VirtualMachineTest, ValidateLDROpCode) {
  // Test of we can load the pc value into r1
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.memory.data_seg_end = 3;
  vm.memory.data_seg_start = 0;
  // load the value 6 into r1
  OperationLDR operation(0x01, 0x00);
  int valid = operation.validate(vm.memory);
  int result = operation.execute(vm.memory);
  int reg_result = vm.memory.registers.getRegister(0x01);
  EXPECT_EQ(reg_result, 6);
  EXPECT_EQ(result, 1);
  EXPECT_EQ(valid, 1);
}

TEST_F(VirtualMachineTest, InvalidCodeSegEnd) {
  // Test if we catch LDR reading outside of data segment
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  // should be 1 byte outside of data after reading the int
  vm.memory.data_seg_end = 3;
  vm.memory.data_seg_start = 0;
  OperationLDR operation(0x01, 0x01);
  EXPECT_THROW(operation.validate(vm.memory), MemoryException);
}

TEST_F(VirtualMachineTest, InvalidCodeSegStart) {
  // Test if we catch LDR reading before data segment
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  // should be 1 byte outside of data after reading the int
  vm.memory.data_seg_end = 3;
  vm.memory.data_seg_start = 1;
  OperationLDR operation(0x01, 0x00);
  EXPECT_THROW(operation.validate(vm.memory), MemoryException);
}

TEST_F(VirtualMachineTest, ValidateLDBOpCode) {
  // Test if we can load 'e' from we.bin into register1
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.memory.data_seg_end = 5;
  OperationLDB operation(0x01, 5);

  int valid = operation.validate(vm.memory);
  int result = operation.execute(vm.memory);
  int reg_result = vm.memory.registers.getRegister(0x01);

  EXPECT_EQ(reg_result, 'e');
  EXPECT_EQ(result, 1);
  EXPECT_EQ(valid, 1);

  // should be 1 byte outside of data after reading the int
}

// Tests for trap operation at to test the execute in Virtual Machine
TEST_F(VirtualMachineTest, ValidateFetchDecodeExcuteFlow) {
  // Test the fetch decode excute for virtual machine
  // Want to test two valid instructions by validating them and then executing
  // them
}
