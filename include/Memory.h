#ifndef MEMORY_H
#define MEMORY_H

#include "Registers.h"
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
  unsigned int pc = 0;
  unsigned int code_seg_start = 0;
  unsigned int code_seg_end = 0;
  unsigned int data_seg_start = 0x04;
  unsigned int data_seg_end = 0;
  static const int size = 1000000; // 1MB
  Registers registers;

private:
  unsigned char memory[size];
};

#endif
