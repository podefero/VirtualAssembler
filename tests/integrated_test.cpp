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

            while (memory_size > vm->memory.registers.getRegister(Registers::PC)) {
                if (vm->fetch() != 1) {
                    std::cerr << "failed to fetch";
                }
                if (vm->decode() != 1) {
                    std::cerr << "failed to decode";
                }
            }

            vm->initPc(); // reset pc
            // execution process. second pass.
            while (memory_size > vm->memory.registers.getRegister(Registers::PC)) {
                if (vm->fetch() != 1)
                    std::cerr << "failed to fetch";
                if (vm->decode() != 1)
                    std::cerr << "failed to decode";
                int result = vm->execute();
                // if (result == 2)
                //  std::cout << std::endl << "Successful exit" << std::endl;
                if (result == -1) {
                    std::cout << "Execute failed " << std::endl;
                }
            }
        } catch (const MemoryException &ex){
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
//Call the function twice with different params, outputs the result each time
TEST_F(Integrated, AddFunctionASM) {
    asmFile = "../../asm/test/valid/AddFunction.asm";
    binFile = "../../asm/test/valid/AddFunction.bin";
    RunAssembly(false);
    RunVm(false);
}

