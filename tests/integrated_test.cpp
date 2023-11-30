#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "PassOneException.h"
#include "Assembly.h"

//Fixture for testing asm and bin files
class Integrated : public testing::Test {
protected:
    void SetUp() override {
        vm = std::make_unique<VirtualMachine>();
        am = std::make_unique<Assembly>();
    }

public:
    std::unique_ptr<VirtualMachine> vm;
    std::unique_ptr<Assembly> am;
    std::string asmFile;
    std::string binFile;

    void RunAssembly(bool expectThrow) const {
        if (expectThrow) {
            EXPECT_ANY_THROW({
                                 am->passOne(asmFile);
                                 am->passTwo();
                             });
        } else {
            EXPECT_NO_THROW({
                                am->passOne(asmFile);
                                am->passTwo();
                            });
        }
    }

    void HelperRunVM() const {

        try {
            vm->loadMemory(binFile);
            vm->initPc();
            int memory_size = vm->findTrap0();
            vm->setStackPointers(memory_size);

            // validate opcodes by fetch and decode, the second pass we will
            // execute.
            while (memory_size > vm->memory.registers.getRegister(Registers::PC)) {
                try {
                    vm->fetch();
                    vm->decode();
                } catch (const MemoryException &ex) {
                    vm->memory.registers.dumpRegisters();
                    std::cerr << "Code Seg Start: " << vm->memory.code_seg_start << std::endl;
                    std::cerr << "Code Seg End " << vm->memory.code_seg_end << std::endl;
                    std::cerr << std::endl;
                    std::cerr << "Data Seg Start " << vm->memory.data_seg_start << std::endl;
                    std::cerr << "Data Seg End " << vm->memory.data_seg_end << std::endl;
                    std::cerr << ex.what() << std::endl;
                }
            }

            vm->initPc(); // reset pc
            // execution process. second pass.
            while (memory_size > vm->memory.registers.getRegister(Registers::PC)) {
                try {
                    vm->fetch();
                    vm->decode();
                    vm->execute();
                } catch (const MemoryException &ex) {
                    vm->memory.registers.dumpRegisters();
                    std::cerr << "Code Seg Start: " << vm->memory.code_seg_start << std::endl;
                    std::cerr << "Code Seg End " << vm->memory.code_seg_end << std::endl;
                    std::cerr << std::endl;
                    std::cerr << "Data Seg Start " << vm->memory.data_seg_start << std::endl;
                    std::cerr << "Data Seg End " << vm->memory.data_seg_end << std::endl;
                    std::cerr << ex.what() << std::endl;
                } catch (const TrapException &ex) {

                }
            }
        } catch (const MemoryException &ex) {
            throw MemoryException(ex.what());
        }

    }

    void RunVm(bool expectThrow) const {
        if (expectThrow) {
            EXPECT_THROW(HelperRunVM(), MemoryException);
        } else {
            EXPECT_NO_THROW(HelperRunVM());
        }
    }
};

TEST_F(Integrated, InvalidTrap0) {
    asmFile = "../../asm/test/invalid/weNoCode.asm";
    binFile = "../../asm/test/invalid/weNoCode.bin";
    RunAssembly(true);
}

//making sure my tests work
TEST_F(Integrated, ValidTrap0) {
    asmFile = "../../asm/test/valid/we.asm";
    binFile = "../../asm/test/valid/we.bin";
    RunAssembly(false);
    RunVm(false);
}

//AddFunction.asm
//Call add function add(5,3)
TEST_F(Integrated, AddFunctionASM) {
    asmFile = "../../asm/test/valid/AddFunction.asm";
    binFile = "../../asm/test/valid/AddFunction.bin";
    RunAssembly(false);
    RunVm(false);
}

//Fib.asm
//testing recursion
//Call fib function fib(7)
TEST_F(Integrated, FibFunctionASM) {
    asmFile = "../../asm/test/valid/Fib.asm";
    binFile = "../../asm/test/valid/Fib.bin";
    RunAssembly(false);
    RunVm(false);
}

//BranchingTest.asm
//needs user input, handle that later on
//TEST_F(Integrated, BranchASM) {
//    asmFile = "../../asm/test/valid/BranchingTest.asm";
//    binFile = "../../asm/test/valid/BranchingTest.bin";
//    RunAssembly(false);
//    RunVm(false);
//}

//HelloWorld.asm
TEST_F(Integrated, HelloWorldASM) {
    asmFile = "../../asm/test/valid/HelloWorld.asm";
    binFile = "../../asm/test/valid/HelloWorld.bin";
    RunAssembly(false);
    RunVm(false);
}

//Proj3_DAGSGADS.asm
TEST_F(Integrated, DagsGadsASM) {
    asmFile = "../../asm/test/valid/Project3_DagsGads.asm";
    binFile = "../../asm/test/valid/Project3_DagsGads.bin";
    RunAssembly(false);
    RunVm(false);
}

//Proj3_OddsEvens.asm
TEST_F(Integrated, OddsEvens) {
    asmFile = "../../asm/test/valid/Project3_OddsEvens.asm";
    binFile = "../../asm/test/valid/Project3_OddsEvens.bin";
    RunAssembly(false);
    RunVm(false);
}




