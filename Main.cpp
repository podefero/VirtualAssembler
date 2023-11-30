#include "Assembly.h"
#include "PassOneException.h"
#include "PassTwoException.h"
#include "VirtualMachine.h"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::string fileExtension = std::filesystem::path(filePath).extension();

    if (fileExtension == ".asm") {
        Assembly assembly;
        try {
            assembly.passOne(filePath);
        } catch (const PassOneException &ex) {
            std::cerr << ex.what() << std::endl;
        }
        try {
            assembly.passTwo();
        } catch (const PassTwoException &ex) {
            std::cerr << ex.what() << std::endl;
        }
    } else if (fileExtension == ".bin") {
        // Input file is a binary file, execute it using the VirtualMachine
        try {
            VirtualMachine vm;
            int result_file = vm.loadMemory(filePath);

            if (result_file == 1) {
                vm.initPc();
                int memory_size = vm.findTrap0();
                vm.setStackPointers(memory_size);
                if (memory_size == 1)
                    return 1;

                // validate opcodes by fetch and decode, the second pass we will
                // execute.
                while (memory_size > vm.memory.registers.getRegister(Registers::PC)) {
                    try {
                        vm.fetch();
                        vm.decode();
                    } catch (const MemoryException &ex) {
                        vm.memory.registers.dumpRegisters();
                        std::cerr << "Code Seg Start: " << vm.memory.code_seg_start << std::endl;
                        std::cerr << "Code Seg End " << vm.memory.code_seg_end << std::endl;
                        std::cerr << std::endl;
                        std::cerr << "Data Seg Start " << vm.memory.data_seg_start << std::endl;
                        std::cerr << "Data Seg End " << vm.memory.data_seg_end << std::endl;
                        std::cerr << ex.what() << std::endl;
                        return -1;
                    }
                }

                vm.initPc(); // reset pc
                // execution process. second pass.
                while (memory_size > vm.memory.registers.getRegister(Registers::PC)) {
                    try {
                        vm.fetch();
                        vm.decode();
                        vm.execute();
                    } catch (const MemoryException &ex) {
                        vm.memory.registers.dumpRegisters();
                        std::cerr << "Code Seg Start: " << vm.memory.code_seg_start << std::endl;
                        std::cerr << "Code Seg End " << vm.memory.code_seg_end << std::endl;
                        std::cerr << std::endl;
                        std::cerr << "Data Seg Start " << vm.memory.data_seg_start << std::endl;
                        std::cerr << "Data Seg End " << vm.memory.data_seg_end << std::endl;
                        std::cerr << ex.what() << std::endl;
                        return -1;
                    } catch (const TrapException &ex) {
                        return 1;
                    }
                }
            } else {
                std::cerr << "Failed to load memory." << std::endl;
            }
        } catch (const std::exception &ex) {
            std::cerr << "An exception occurred: " << ex.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Unsupported file extension: " << fileExtension << std::endl;
        return 1;
    }

    return 0;
}
