#include "TokenInt.h"

TokenInt::TokenInt(int value) { this->value = value; }

void TokenInt::validate(std::map<std::string, unsigned int> &symbol_table,
                        unsigned int &limit) {}

std::vector<unsigned char> TokenInt::getBytes() {
  std::vector<unsigned char> bytes;
  bytes.push_back(static_cast<unsigned char>(value & 0xFF));
  bytes.push_back(static_cast<unsigned char>((value >> 8) & 0xFF));
  bytes.push_back(static_cast<unsigned char>((value >> 16) & 0xFF));
  bytes.push_back(static_cast<unsigned char>((value >> 24) & 0xFF));
  return bytes;
}
