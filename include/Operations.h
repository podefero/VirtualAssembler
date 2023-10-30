#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Memory.h"
#include "Registers.h"
#include "MemoryException.h"
#include "TrapException.h"

class Operation {
public:
    virtual ~Operation() = default;

    Operation(int op1, int op2) {
        operand1 = op1;
        operand2 = op2;
    }

    virtual int validate(Memory &memory) = 0;

    virtual int execute(Memory &memory) = 0;

protected:
    int operand1 = 0;
    int operand2 = 0;
};

class OperationAdd : public Operation {
public:
    OperationAdd(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        // op1 is destination op2 is source
        try {
            int rd = memory.registers.getRegister(operand1);
            int rs = memory.registers.getRegister(operand2);
            rd += rs;
            memory.registers.setRegister(operand1, rd);
        } catch (const std::out_of_range &ex) {
            std::cerr << ex.what();
            return -1;
        }
        return 1;
    }
};

class OperationDiv : public Operation {
public:
    OperationDiv(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        // op1 is destination op2 is source
        try {
            int rd = memory.registers.getRegister(operand1);
            int rs = memory.registers.getRegister(operand2);
            rd /= rs;
            memory.registers.setRegister(operand1, rd);
        } catch (const std::out_of_range &ex) {
            return -1;
        }
        return 1;
    }
};

class OperationJump : public Operation {
public:
    OperationJump(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override {
        if (operand1 <= memory.data_seg_end) {
            throw (MemoryException(
                    "Jump is in data segment space, must be in code segment space"));
        } // Check if we are out of code_seg space
        else if (operand1 >= memory.code_seg_end) {
            throw (MemoryException("Jump is out of memory space"));
        }
        return 1;
    }

    int execute(Memory &memory) override {
        memory.pc = operand1;
        return 1;
    }
};

class OperationLDB : public Operation {
public:
    OperationLDB(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override {
        // check if we are out of data_seg space
        if (operand2 > memory.data_seg_end) {
            throw (MemoryException("LDB is past data segment"));
        } // Check if we are before data_seg
        else if (operand2 < memory.data_seg_start) {
            throw (MemoryException("LDB is before data segment"));
        }
        return 1;
    }

    int execute(Memory &memory) override {
        try {
            // byte directive
            memory.registers.setRegister(operand1, memory.readByte(operand2));
        } catch (const std::out_of_range &ex) {
            return -1;
        } catch (const MemoryException &ex) {
            return -1;
        }
        return 1;
    }
};

class OperationLDR : public Operation {
public:
    OperationLDR(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override {
        // check if we are out of data_seg space
        // not counting the zero so minus 1
        if ((operand2 + sizeof(int)) - 1 > memory.data_seg_end) {
            throw (MemoryException("LDR is past data segment"));
        } // Check if we are before data_seg
        else if (operand2 < memory.data_seg_start) {
            throw (MemoryException("LDR is before data segment"));
        }
        return 1;
    }

    int execute(Memory &memory) override {
        try {
            // int directive
            memory.registers.setRegister(operand1, memory.readInt(operand2));
        } catch (const std::out_of_range &ex) {
            return -1;
        } catch (const MemoryException &ex) {
            return -1;
        }

        return 1;
    }
};

class OperationMove : public Operation {
public:
    OperationMove(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        try {
            int rs = memory.registers.getRegister(operand2);
            memory.registers.setRegister(operand1, rs);
        } catch (const std::out_of_range &ex) {
            return -1;
        }
        return 1;
    }
};

class OperationMul : public Operation {
public:
    OperationMul(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        // op1 is destination op2 is source
        try {
            int rd = memory.registers.getRegister(operand1);
            int rs = memory.registers.getRegister(operand2);
            rd *= rs;
            memory.registers.setRegister(operand1, rd);
        } catch (const std::out_of_range &ex) {
            return -1;
        }
        return 1;
    }
};

class OperationSTB : public Operation {
public:
    OperationSTB(int op1, int op2) : Operation(op1, op2) {};

    int validate(Memory &memory) override {
        // check if we are out of data_seg space
        // not counting the zero so minus 1
        if ((operand2 + sizeof(char)) - 1 > memory.data_seg_end) {
            throw (MemoryException("STB is past data segment"));
        } // Check if we are before data_seg
        else if (operand2 < memory.data_seg_start) {
            throw (MemoryException("STB is before data segment"));
        }
        return 1;
    }

    int execute(Memory &memory) override {
        try {
            // get regValue
            unsigned int rs = memory.registers.getRegister(operand1);
            unsigned int offset = operand2;
            // write rs to label
            memory.writeByte(offset, static_cast<unsigned char>(rs & 0xFF));
        } catch (const std::out_of_range &ex) {
            return -1;
        } catch (const MemoryException &ex) {
            return -1;
        }

        return 1;
    }
};

class OperationSTR : public Operation {
public:
    OperationSTR(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override {
        // check if we are out of data_seg space
        // not counting the zero so minus 1
        if ((operand2 + sizeof(int)) - 1 > memory.data_seg_end) {
            throw (MemoryException("STR is past data segment"));
        } // Check if we are before data_seg
        else if (operand2 < memory.data_seg_start) {
            throw (MemoryException("STR is before data segment"));
        }
        return 1;
    }

    int execute(Memory &memory) override {
        try {
            // get regValue
            unsigned int rs = memory.registers.getRegister(operand1);
            unsigned int offset = operand2;
            // write rs to label
            memory.writeInt(offset, rs);
        } catch (const std::out_of_range &ex) {
            return -1;
        } catch (const MemoryException &ex) {
            return -1;
        }
        return 1;
    }
};

class OperationSub : public Operation {
public:
    OperationSub(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        try {
            int rd = memory.registers.getRegister(operand1);
            int rs = memory.registers.getRegister(operand2);
            rd -= rs;
            memory.registers.setRegister(operand1, rd);
        } catch (const std::out_of_range &ex) {
            return -1;
        }
        return 1;
    }
};

class OperationTrap0 : public Operation {
public:
    OperationTrap0(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        throw (TrapException("TRAP INTERRUPT"));
    }
};

class OperationTrap1 : public Operation {
public:
    OperationTrap1(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        // throw(TrapException("TRAP 1 INTERRUPT"));
        // print r3 to console
        int r3 = memory.registers.getRegister(3);
        std::cout << r3;
        return 1; // return 1 for trap 1
    }
};

class OperationTrap2 : public Operation {
public:
    OperationTrap2(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        unsigned char value;
        std::cin >> value;
        if (std::cin.fail()) {
            throw MemoryException("Trap2 Invalid input");
        } else
            memory.registers.setRegister(3, value);
        return 2; // return 2 for trap 2
    }
};

class OperationTrap3 : public Operation {
public:
    OperationTrap3(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        int r3 = memory.registers.getRegister(3);
        char byte = static_cast<char>(r3 & 0xFF);
        std::cout << byte;
        return 3; // return 3 for trap 3
    }
};

class OperationTrap4 : public Operation {
public:
    OperationTrap4(int op1, int op2) : Operation(op1, op2) {}

    int validate(Memory &memory) override { return 1; }

    int execute(Memory &memory) override {
        int value;
        std::cin >> value;
        if (std::cin.fail()) {
            throw MemoryException("Trap4 Invalid input");
        } else
            memory.registers.setRegister(3, value);
        return 4; // return 4 for trap 4
    }
};

#endif  
