#include "VirtualMachine.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
    return 1;
  }

  std::string filePath = argv[1];

  try {
    VirtualMachine vm;
    int result = vm.loadMemory(filePath);
    vm.initPc();
    if (result == 1) {
      std::cout << "Memory loaded successfully." << std::endl;
      std::cout << vm.memory.readInt(0) << " PC " << std::endl;
      std::cout << sizeof(int) << " sizeof int" << std::endl;
      std::cout << vm.findTrap0() << std::endl;
      // std::cout << size_t(int) << " size_t int" << std::endl;

    } else {
      std::cerr << "Failed to load memory." << std::endl;
    }
  } catch (const std::exception &ex) {
    std::cerr << "An exception occurred: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
