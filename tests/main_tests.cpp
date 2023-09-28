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
  EXPECT_EQ(expect, vm.getPc());
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
  EXPECT_EQ(vm.getPc(), expect_pc);
}

TEST_F(VirtualMachineTest, FetchNDecode) {
  // Test the decode, see if we can add the AddOperation
  std::string filePath = "we.bin";
  vm.loadMemory(filePath);

  vm.initPc();

  // set instruction and updated pc
  vm.fetch();

  vm.decode();

  EXPECT_FALSE(vm.getOperationQueue().empty());
}

TEST_F(VirtualMachineTest, ValidateAddOpCode) {
  // Test the add opcode
  Registers registers;
  registers.setRegister(0x01, 10);
  registers.setRegister(0x02, 10);

  // reg 0x01 should be 20 now
  OperationAdd operation(0x01, 0x02);
  int result = operation.execute(registers);
  int reg_result = 20;
  EXPECT_EQ(result, 1);
  EXPECT_EQ(reg_result, registers.getRegister(0x01));
}
