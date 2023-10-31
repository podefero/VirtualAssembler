#include "Memory.h"
#include "MemoryException.h"
#include <iostream>
Memory::Memory() = default;

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
    // std::cout << static_cast<int>(memory[offset]) << "Byte value";
    result = memory[offset];
  }
  return result;
}

Memory::Instruction Memory::readInstruction(unsigned int offset) {
  Instruction instruction{};
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
void Memory::writeInt(unsigned int offset, unsigned int value) {
  if (offset + sizeof(unsigned int) > size) {
    throw MemoryException("Can't write int, beyond memory size. Offset: " +
                          std::to_string(offset));
  } else {
    // write 4 bytes starting with offset
    for (int i = 0; i < sizeof(unsigned int); ++i) {
      memory[offset + i] = value & 0xFF;
      value >>= 8;
    }
  }
}

void Memory::writeByte(unsigned int offset, unsigned char value) {
  if (offset + sizeof(unsigned char) > size) {
    throw MemoryException(
        "Can't write unsigned char beyond memory size. Offset :" +
        std::to_string(offset));
  } else {
    // write 1 byte starting with offset
    memory[offset] = value;
  }
}

void Memory::writeInstruction(unsigned int offset, Instruction instruction) {
  if (offset + sizeof(Instruction) > size) {
    throw MemoryException(
        "Can't write instruction beyond memory size. Offset :" +
        std::to_string(offset));
  } else {
    // write instruction (12 bytes)
    writeInt(offset, instruction.opcode);
    writeInt(offset + sizeof(int), instruction.operand1);
    writeInt(offset + sizeof(int) * 2, instruction.operand2);
  }
}
