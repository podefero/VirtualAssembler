#include "Memory.h"
#include "MemoryException.h"
#include <iostream>
Memory::Memory() {}

// loads binary file into memory array
void Memory::loadFromFile(const std::string &filePath) {
  std::ifstream file(filePath, std::ios::binary | std::ios::in);

  if (!file.is_open()) {
    throw MemoryException("Failed to open file: " + filePath);
  } else {
    // Read binary data from the file into the memory array.
    file.read(reinterpret_cast<char *>(memory), size);
    file.close();
  }
}

// return the value read into an unsigned int
unsigned int Memory::readInt(unsigned int offset) {
  unsigned int result = 0;
  if (offset + sizeof(unsigned int) > size) {
    throw MemoryException("Can't read int, beyond memory size. Offset: " +
                          std::to_string(offset));
  } else {
    // read in 4 bytes staring with offset
    for (int i = sizeof(unsigned int) - 1; i >= 0; --i) {
      result = (result << 8) | memory[offset + i];
    }
  }

  return result;
}

unsigned char Memory::readByte(unsigned int offset) {
  unsigned char result = 0;
  if (offset + sizeof(unsigned char) > size) {
    throw MemoryException(
        "Can't read unsigned char beyond memory size. Offset :" +
        std::to_string(offset));
  } else {
    // read in 1 byte starting with offset.
    result = memory[offset];
  }
  return result;
}

Memory::Instruction Memory::readInstruction(unsigned int offset) {
  Instruction instruction;
  if (offset + sizeof(Instruction) > size) {
    throw MemoryException("Cant read instruction beyond memory size. Offset :" +
                          std::to_string(offset));
  } else {
    // read in instruction. 12 bytes
    instruction.opcode = readInt(offset);
    instruction.operand1 = readInt(offset + sizeof(int));
    instruction.operand2 = readInt(offset + sizeof(int) * 2);
  }
  return instruction;
}
