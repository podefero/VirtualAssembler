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

        if (vm.fetch() != 1)
          std::cerr << "failed to fetch";
        if (vm.decode() != 1)
          std::cerr << "failed to decode";
        int result = vm.execute();
        if (result == 2)
          std::cout << std::endl << "Successful exit" << std::endl;
        else if (result != 1) {
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

  return 0;
}
