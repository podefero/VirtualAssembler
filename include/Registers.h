#ifndef REGISTERS_H
#define REGISTERS_H

class Registers {
private:
  static const int NUM_REGISTERS = 16;
  int registers[NUM_REGISTERS];

public:
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
