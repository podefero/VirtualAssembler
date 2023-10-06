#ifndef REGISTERS_H
#define REGISTERS_H

#include <iostream>

class Registers {
private:
  static const int NUM_REGISTERS = 16;
  int registers[NUM_REGISTERS];

public:
  Registers() {
    for (int i = 0; i < NUM_REGISTERS; i++) {
      registers[i] = 0;
    }
  }

  int getRegister(int index) {
    if (index < 0 || index >= NUM_REGISTERS) {
      throw std::out_of_range("Invalid register index");
    }
    return registers[index];
  }

  void setRegister(int index, int value) {
    if (index < 0 || index >= NUM_REGISTERS) {
      throw std::out_of_range("Invalid register index");
    }
    registers[index] = value;
  }
};

#endif
