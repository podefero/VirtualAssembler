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
    // Initialize. Need to make a init func that handles this
    int result = vm.loadMemory(filePath);

    if (result == 1) {
      vm.initPc();
      int memory_size = vm.findTrap0();
      // if we find no trap then exit;
      if (memory_size == 1)
        return 1;
      // std::cout << memory_size << std::endl;
      while (memory_size > vm.memory.pc) {
        vm.fetch();
        vm.decode();
      }

      vm.execute();

    } else {
      std::cerr << "Failed to load memory." << std::endl;
    }
  } catch (const std::exception &ex) {
    std::cerr << "An exception occurred: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}
