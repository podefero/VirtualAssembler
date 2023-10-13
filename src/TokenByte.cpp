#include "TokenByte.h"

TokenByte::TokenByte(unsigned int offset, unsigned char value) {
  this->offset = offset;
  this->value = value;
}

void TokenByte::validate(unsigned int &offset) {}

std::vector<unsigned char> TokenByte::getBytes() {
  std::vector<unsigned char> bytes;
  bytes[0] = value;
  return bytes;
}
