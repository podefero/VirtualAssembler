#include "binary_converter.h"
#include <iostream>
#include <string>

std::string int_to_binary(int num) {
  // Convert the integer to binary and output as a string
  std::string binary;
  while (num > 0) {
    binary = std::to_string(num % 2) + binary;
    num /= 2;
  }

  return binary;
}
bool is_valid_input(std::string input) {

  // Validate input
  for (char c : input) {
    if (!isdigit(c)) {
      return false;
    }
  }

  return true;
}

int string_to_int(std::string input) {
  int num;
  try {
    num = std::stoi(input);
  } catch (const std::out_of_range &e) {
    return -1;
  }

  return num;
}
