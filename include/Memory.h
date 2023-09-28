#ifndef MEMORY_H
#define MEMORY_H

#include <fstream>
#include <string>

class Memory {
public:
  Memory();
  void loadFromFile(const std::string &filePath);
  unsigned int readInt(unsigned int offset);
  unsigned char readByte(unsigned int offset);
  struct Instruction {
    int opcode;
    int operand1;
    int operand2;
  };
  Instruction readInstruction(unsigned int offset);

private:
  static const int size = 80; // 1MB
  unsigned char memory[size];
};

#endif
