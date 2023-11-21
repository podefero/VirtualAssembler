#ifndef REGISTERS_H
#define REGISTERS_H

#include <iostream>

class Registers {
private:
    static const int NUM_REGISTERS = 22;
    int registers[NUM_REGISTERS]{};

public:
    static const int NUM_RG = 15; //0 - 15
    static const int PC = 16;
    static const int SL = 17;
    static const int SB = 18;
    static const int SP = 19;
    static const int FP = 20;
    static const int HP = 21;

    Registers() {
        for (int & i : registers) {
            i = 0;
        }
    }

    void dumpRegisters() {
        std::cerr << "Registers:\n***\n";
        int count = 0;
        for (int & i : registers) {
            std::cerr << "R" << std::to_string(count) << ": " << std::to_string(i) << std::endl;
            count++;
        }
        std::cerr << "\n***\n";
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
