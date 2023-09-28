#ifndef MEMORYEXCEPTION_H
#define MEMORYEXCEPTION_H

#include <stdexcept>

class MemoryException : public std::runtime_error {
public:
  MemoryException(const std::string &message) : std::runtime_error(message) {}
};

#endif
