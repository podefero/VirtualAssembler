#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Memory.h"
#include "Registers.h"
#include "MemoryException.h"
#include "TrapException.h"

class Operation {
public:
    virtual ~Operation() = default;

    Operation(int opcode, int op1, int op2) {
        this->opcode = opcode;
        operand1 = op1;
        operand2 = op2;
    }

    virtual void validate(Memory &memory) = 0;

    virtual void execute(Memory &memory) = 0;

    //set general register only
    static void setGReg(Memory &memory, int reg, int regVal) {
        if (reg > Registers::NUM_RG)
            throw MemoryException("Out of register range while setting: ");
        memory.registers.setRegister(reg, regVal);
    }

    //get general register only
    static int getGReg(Memory &memory, int reg) {
        if (reg > Registers::NUM_RG)
            throw MemoryException("Out of register range while getting: ");
        return memory.registers.getRegister(reg);
    }

    //set pc only
    static void setPC(Memory &memory, int offset) {
        if (offset > memory.data_seg_end)
            memory.registers.setRegister(Registers::PC, offset);
        else
            throw MemoryException("PC can't be set in data seg ");
    }

    //get pc
    static unsigned int getPC(Memory &memory) {
        return memory.registers.getRegister(Registers::PC);
    }

    //other registers (SP,FP..)
    static unsigned int getSL() {
        //top of stack
        //this is set by vm during runtime
        return 0;
    }
    static unsigned int getSB() {
        //bottom of stack
        //set at runtime by vm
        return 0;
    }

    static void setSP() {
        //move will use this
        //check if it sets above or below limits
    }

    static unsigned int getSP() {
        //stack pointer, points at next free stack frame
        return 0;
    }

    static void setFP() {
        //move will use this
        //check if it sets above SP
        //check if set below limit

    }

    static unsigned int getFP() {
        //points at return address of current activation record on the stack
        return 0;
    }

    //undefined until P5
    static void setHP() {

    }

    static unsigned int getHP() {
        return 0;
    }

    static void isValidCodeSeg(Memory &memory, int offset) {
        if (offset < memory.data_seg_end) {
            throw MemoryException(
                    "Operation is in data segment space, must be in code segment space. Offset " +
                    std::to_string(offset));
        } // Check if we are out of code_seg space
        else if (offset > memory.code_seg_end) {
            throw MemoryException("Operation is out of memory space");
        }
    }

    static void isValidDataSeg(Memory &memory, int offset) {
        if (offset > memory.data_seg_end) {
            throw MemoryException(
                    "Operation is in code segment space, must be in data segment space. Offset: " +
                    std::to_string(offset));
        } // Check if we are out of code_seg space
        else if (offset > memory.code_seg_end) {
            throw MemoryException("Operation is out of memory space");
        }
    }

    [[nodiscard]] std::string operationAsString() const {
        return " " + std::to_string(opcode) + " " + std::to_string(operand1) + " " + std::to_string(operand2) + "\n";
    }

protected:
    int operand1 = 0;
    int operand2 = 0;
    int opcode = 0;
};

class OperationAdd : public Operation {
public:
    OperationAdd(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        int result = rd + rs;
        setGReg(memory, operand1, result);
    }
};

class OperationAddI : public Operation {
public:
    OperationAddI(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int result = rd + operand2;
        setGReg(memory, operand1, result);
    }
};

class OperationDiv : public Operation {
public:
    OperationDiv(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        int result = rd / rs;
        setGReg(memory, operand1, result);
    }
};

class OperationJump : public Operation {
public:
    OperationJump(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidCodeSeg(memory, operand1);
    }

    void execute(Memory &memory) override {
        setPC(memory, operand1);
    }
};

class OperationJMR : public Operation {
public:
    OperationJMR(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        //have to validate here
        isValidCodeSeg(memory, getGReg(memory, operand1));
        setPC(memory, getGReg(memory, operand1));
    }
};

class OperationBNZ : public Operation {
public:
    OperationBNZ(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidCodeSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        //branch to label if operand1 != 0
        if (getGReg(memory, operand1) != 0)
            setPC(memory, operand2);
    }
};

class OperationBGT : public Operation {
public:
    OperationBGT(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidCodeSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        //branch to label if operand1 > 0
        if (getGReg(memory, operand1) > 0)
            setPC(memory, operand2);
    }
};

class OperationBLT : public Operation {
public:
    OperationBLT(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidCodeSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        //branch to label if operand1 < 0
        if (getGReg(memory, operand1) < 0)
            setPC(memory, operand2);
    }
};

class OperationBRZ : public Operation {
public:
    OperationBRZ(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidCodeSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        //branch to label if operand1 != 0
        if (getGReg(memory, operand1) == 0)
            setPC(memory, operand2);
    }
};

class OperationCMP : public Operation {
public:
    OperationCMP(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        //compare
        if (rd == rs) {
            setGReg(memory, operand1, 0);
        } else if (rd > rs) {
            setGReg(memory, operand1, 1);
        } else {
            setGReg(memory, operand1, -1);
        }
    }
};

class OperationCMPI : public Operation {
public:
    OperationCMPI(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int immediate = operand2;
        //compare
        if (rd == immediate) {
            setGReg(memory, operand1, 0);
        } else if (rd > immediate) {
            setGReg(memory, operand1, 1);
        } else {
            setGReg(memory, operand1, -1);
        }
    }
};

class OperationLDB : public Operation {
public:
    OperationLDB(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidDataSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        setGReg(memory, operand1, memory.readByte(operand2));
    }
};

class OperationLDBI : public Operation {
public:
    OperationLDBI(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidDataSeg(memory, getGReg(memory, operand2));
    }

    void execute(Memory &memory) override {
        unsigned int offset = getGReg(memory, operand2);
        setGReg(memory, operand1, memory.readByte(offset));
    }
};

class OperationLDR : public Operation {
public:
    OperationLDR(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidDataSeg(memory, operand2 + sizeof(int) - 1);
    }

    void execute(Memory &memory) override {
        setGReg(memory, operand1, memory.readInt(operand2));
    }
};

class OperationLDRI : public Operation {
public:
    OperationLDRI(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidDataSeg(memory, getGReg(memory, operand2) + sizeof(int) - 1);
    }

    void execute(Memory &memory) override {
        unsigned int offset = getGReg(memory, operand2);
        setGReg(memory, operand1, memory.readInt(offset));
    }
};

class OperationLDA : public Operation {
public:
    OperationLDA(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidDataSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        setGReg(memory, operand1, operand2);
    }
};

class OperationMove : public Operation {
public:
    OperationMove(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        //check if RS is PC
        if (operand2 == 16) {
            setGReg(memory, operand1, getPC(memory));
        } else {
            int rs = getGReg(memory, operand2);
            setGReg(memory, operand1, rs);
        }
    }
};

class OperationMOVI : public Operation {
public:
    OperationMOVI(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        setGReg(memory, operand1, operand2);
    }
};


class OperationMul : public Operation {
public:
    OperationMul(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        int result = rd * rs;
        setGReg(memory, operand1, result);
    }
};

class OperationSTB : public Operation {
public:
    OperationSTB(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {};

    void validate(Memory &memory) override {
        isValidDataSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        unsigned int rs = getGReg(memory, operand1);
        unsigned int offset = operand2;
        memory.writeByte(offset, static_cast<unsigned char>(rs & 0xFF));
    }
};

class OperationSTBI : public Operation {
public:
    OperationSTBI(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {};

    void validate(Memory &memory) override {
        isValidDataSeg(memory, getGReg(memory, operand2));
    }

    void execute(Memory &memory) override {
        unsigned int rs = getGReg(memory, operand1);
        unsigned int offset = getGReg(memory, operand2);
        memory.writeByte(offset, static_cast<unsigned char>(rs & 0xFF));
    }
};

class OperationSTR : public Operation {
public:
    OperationSTR(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidDataSeg(memory, operand2);
    }

    void execute(Memory &memory) override {
        unsigned int rs = getGReg(memory, operand1);
        unsigned int offset = operand2;
        memory.writeInt(offset, rs);
    }
};

class OperationSTRI : public Operation {
public:
    OperationSTRI(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        isValidDataSeg(memory, getGReg(memory, operand2));
    }

    void execute(Memory &memory) override {
        unsigned int rs = getGReg(memory, operand1);
        unsigned int offset = getGReg(memory, operand2);
        memory.writeInt(offset, rs);
    }
};

class OperationSub : public Operation {
public:
    OperationSub(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        int result = rd - rs;
        setGReg(memory, operand1, result);
    }
};

class OperationTrap0 : public Operation {
public:
    OperationTrap0(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        throw (TrapException("TRAP INTERRUPT"));
    }
};

class OperationTrap1 : public Operation {
public:
    OperationTrap1(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        // throw(TrapException("TRAP 1 INTERRUPT"));
        // print r3 to console
        int r3 = getGReg(memory, 3);
        std::cout << r3;
    }
};

class OperationTrap4 : public Operation {
public:
    OperationTrap4(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        unsigned char value;
        std::cin >> value;
        if (std::cin.fail()) {
            throw MemoryException("Trap2 Invalid input");
        } else
            setGReg(memory, 3, value);
    }
};

class OperationTrap3 : public Operation {
public:
    OperationTrap3(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int r3 = getGReg(memory, 3);
        char byte = static_cast<char>(r3 & 0xFF);
        std::cout << byte;
    }
};

class OperationTrap2 : public Operation {
public:
    OperationTrap2(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int value;
        std::cin >> value;
        if (std::cin.fail()) {
            throw MemoryException("Trap4 Invalid input");
        } else
            setGReg(memory, 3, value);
    }
};

#endif  
