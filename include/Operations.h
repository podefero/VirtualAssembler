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
    static unsigned int getSL(Memory &memory) {
        //top of stack
        //this is set by vm during runtime
        return memory.registers.getRegister(Registers::SL);
    }

    static unsigned int getSB(Memory &memory) {
        //bottom of stack
        //set at runtime by vm
        return memory.registers.getRegister(Registers::SB);
    }

    static void setSP(Memory &memory, int offset) {
        //move will use this
        //check if it sets above or below limits
//        if (offset < getSL(memory) || offset > getSB(memory)) {
//            throw MemoryException("SP can't be set outside of SL or SB");
//        }
        memory.registers.setRegister(Registers::SP, offset);
    }

    static unsigned int getSP(Memory &memory) {
        //stack pointer, points at next free stack frame
        return memory.registers.getRegister(Registers::SP);
    }

    static void setFP() {
        //move will use this
        //check if it sets above SP
        //check if set below limit

    }

    static unsigned int getFP(Memory &memory) {
        //points at return address of current activation record on the stack
        return memory.registers.getRegister(Registers::FP);
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

    static void printStack(Memory &memory) {
        unsigned int sp = getSP(memory);
        unsigned int sb = getSB(memory);
        unsigned int sl = getSL(memory);
        unsigned int fp = getFP(memory);
        unsigned int size = (sb - sp) / 4;
        std::cout << "SP " << sp << std::endl;
        std::cout << "FP " << fp << std::endl;
        std::cout << "SL " << sl << std::endl;
        std::cout << "SB " << sb << std::endl;
        unsigned address = (sp) + 4; //start at address 0
        while (sp < sb) {
            sp += 4;
            int stack_element = memory.readInt(sp);
            std::cout << address << ": " << stack_element << std::endl;
            address += 4;
        }
        std::cout << "Size " << size << std::endl;
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
        //isValidDataSeg(memory, getGReg(memory, operand2));
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
        //isValidDataSeg(memory, getGReg(memory, operand2) + sizeof(int) - 1);
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

    void validate(Memory &memory) override {
        //cant set pc
        if (operand1 == 16)
            throw MemoryException("Can't set PC");
    }

    void execute(Memory &memory) override {
        int rs = memory.registers.getRegister(operand2);
        memory.registers.setRegister(operand1, rs);
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
        //isValidDataSeg(memory, getGReg(memory, operand2));
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
        //isValidDataSeg(memory, getGReg(memory, operand2));
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

class OperationAND : public Operation {
public:
    OperationAND(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        if (rd == 1 && rs == 1) {
            setGReg(memory, operand1, 1);
        } else {
            setGReg(memory, operand1, -1);
        }
    }
};

class OperationOR : public Operation {
public:
    OperationOR(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        if (rd == 1 || rs == 1) {
            setGReg(memory, operand1, 1);
        } else {
            setGReg(memory, operand1, -1);
        }
    }
};

class OperationNOT : public Operation {
public:
    OperationNOT(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        int rd = getGReg(memory, operand1);
        int rs = getGReg(memory, operand2);
        if (!(rd == 1 || rs == 1)) {
            setGReg(memory, operand1, 1);
        } else {
            setGReg(memory, operand1, -1);
        }
    }
};

class OperationPUSH : public Operation {
public:
    OperationPUSH(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        if (getSP(memory) < getSL(memory)) {
            printStack(memory);
            throw MemoryException(
                    "Stack Overflow - Can't push register: " + std::to_string(operand1) + " Value: " + std::to_string(
                            getGReg(memory, operand1)) + "\n SP: " + std::to_string(getSP(memory)) + "\n SL: " +
                    std::to_string(
                            getSL(memory)));
        }

    }

    void execute(Memory &memory) override {
        int sp = getSP(memory);
        int rg = getGReg(memory, operand1);
        memory.writeInt(sp, rg);
        setSP(memory, sp - 4);
    }
};

class OperationPOP : public Operation {
public:
    OperationPOP(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {
        if (getSP(memory) >= getSB(memory)) {
            throw MemoryException("Stack Underflow");
        }
    }

    void execute(Memory &memory) override {
        int sp = getSP(memory) + 4;
        int value = memory.readInt(sp);
        setGReg(memory, operand1, value);
        setSP(memory, sp);
    }
};

class OperationPEEK : public Operation {
public:
    OperationPEEK(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {

    }

    void execute(Memory &memory) override {
        int sp = getSP(memory) + 4; //looking at value below sp
        int value = memory.readInt(sp);
        setGReg(memory, operand1, value);
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

class OperationTrap5 : public Operation {
public:
    OperationTrap5(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    void execute(Memory &memory) override {
        //be sure to load a pointer not a value
        int r3 = getGReg(memory, 3);

        //use offset to move ptr
        unsigned int offset = r3;

        //get length
        unsigned int length = memory.readInt(offset);

        //update offset
        offset += 4;

        for (int i = 0; i < length; ++i) {
            unsigned char byte = memory.readByte(offset);
            offset += 1;
            std::cout << byte;
        }
    }
};

class OperationTrap99 : public Operation {
public:
    OperationTrap99(int opcode, int op1, int op2) : Operation(opcode, op1, op2) {}

    void validate(Memory &memory) override {}

    //print out stack
    void execute(Memory &memory) override {
        unsigned int sp = getSP(memory);
        unsigned int sb = getSB(memory);
        unsigned int sl = getSL(memory);
        unsigned int fp = getFP(memory);
        unsigned int size = (sb - sp) / 4;
        std::cout << "SP " << sp << std::endl;
        std::cout << "FP " << fp << std::endl;
        std::cout << "SL " << sl << std::endl;
        std::cout << "SB " << sb << std::endl;
        unsigned address = (sp) + 4; //start at address 0
        while (sp < sb) {
            sp += 4;
            int stack_element = memory.readInt(sp);
            std::cout << address << ": " << stack_element << std::endl;
            address += 4;
        }
        std::cout << "Size " << size << std::endl;
    }
};

#endif
