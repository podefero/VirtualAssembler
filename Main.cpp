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
      int result = vm.loadMemory(filePath);

      if (result == 1) {
        vm.initPc();
        int memory_size = vm.findTrap0();
        vm.setStackPointers(memory_size);
        if (memory_size == 1)
          return 1;

        // validate opcodes by fetch and decode, the second pass we will
        // execute.
        while (memory_size > vm.memory.registers.getRegister(Registers::PC)) {
          if (vm.fetch() != 1) {
            std::cerr << "failed to fetch";
            return 1;
          }
          if (vm.decode() != 1) {
            std::cerr << "failed to decode";
            return 1;
          }
        }

        vm.initPc(); // reset pc
        // execution process. second pass.
        while (memory_size > vm.memory.registers.getRegister(Registers::PC)) {
          if (vm.fetch() != 1)
            std::cerr << "failed to fetch";
          if (vm.decode() != 1)
            std::cerr << "failed to decode";
          int result = vm.execute();
          // if (result == 2)
          //  std::cout << std::endl << "Successful exit" << std::endl;
          if (result == -1) {
            std::cout << "Execute failed " << std::endl;
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
