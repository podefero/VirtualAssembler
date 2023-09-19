#include <iostream>
#include <mpark/patterns.hpp>

enum class Opcode { ADD, SUB, MUL };

void emulate(Opcode opcode, int operand1, int operand2) {
  using namespace mpark::patterns;
  match(opcode)(
      pattern(Opcode::ADD) =
          [operand1, operand2] {
            std::cout << operand1 + operand2 << std::endl;
          },
      pattern(Opcode::SUB) =
          [operand1, operand2] {
            std::cout << operand1 - operand2 << std::endl;
          },
      pattern(_) = [] { std::cout << "Unknown opcode" << std::endl; });
}

int main() {
  emulate(Opcode::ADD, 5, 3); // Output: 8
  emulate(Opcode::SUB, 8, 2); // Output: 6
  emulate(Opcode::MUL, 2, 4); // Output: Unknown opcode
  return 0;
}
