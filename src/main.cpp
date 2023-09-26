#include "DirectiveInt.h"
#include <iostream>

int main() {
  DirectiveInt directive(10, 100);

  unsigned int offset = directive.get_offset();
  int value = directive.get_value();

  std::cout << "Offset: " << offset << std::endl;
  std::cout << "Value: " << value << std::endl;

  return 0;
}
