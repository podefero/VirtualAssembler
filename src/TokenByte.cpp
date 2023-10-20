#include "TokenByte.h"

TokenByte::TokenByte(unsigned char value) { this->value = value; }

void TokenByte::validate(std::map<std::string, unsigned int> &symbol_table,
                         unsigned int &limit) {}

std::vector<unsigned char> TokenByte::getBytes() {
  std::vector<unsigned char> bytes;
  bytes.push_back(value);
  return bytes;
}
