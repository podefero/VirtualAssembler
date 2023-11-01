#include "MemoryException.h"
#include "TrapException.h"
#include "VirtualMachine.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(OPTEST, MOV) {
    Memory memory;

    //set pc
    memory.registers.setRegister(Registers::PC, 25);
    //move pc to R1
    OperationMove move(7, 1, 16);
    move.execute(memory);

    //R1 should have 25 in it.
    int result = memory.registers.getRegister(1);
    EXPECT_EQ(25, result);
}