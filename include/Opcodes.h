#ifndef OPCODES_H
#define OPCODES_H

enum class OpCode : int {
    MOV = 7,
    MOVI = 31,
    LDA = 8,
    STR = 9,
    STRI = 22,
    LDR = 10,
    LDRI = 23,
    STB = 11,
    STBI = 24,
    LDB = 12,
    LDBI = 25,
    AND = 18,
    OR = 19,
    NOT = 39,
    CMP = 20,
    CMPI = 32,
    ADD = 13,
    ADDI = 14,
    SUB = 15,
    MUL = 16,
    MULI = 33,
    DIV = 17,
    DIVI = 34,
    ALCI = 35,
    ALLC = 36,
    ALLCI = 37,
    FREE = 38,
    TRAP = 21,
    JMP = 1,
    JMR = 2,
    BNZ = 3,
    BGT = 4,
    BLT = 5,
    BRZ = 6

};

class OpcodeUtil {
public :
     static int getOpcode(OpCode opCode){
        return static_cast<int>(opCode);
}
};
#endif
