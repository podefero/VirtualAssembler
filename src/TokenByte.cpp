#include "TokenByte.h"

TokenByte::TokenByte(unsigned int offset, unsigned char value) {
  this->offset = offset;
  this->value = value;
}

void TokenByte::validate(std::map<std::string, unsigned int> &symbol_table) {}

std::vector<unsigned char> TokenByte::getBytes() {
  std::vector<unsigned char> bytes;
  bytes.push_back(value);
  return bytes;
}
