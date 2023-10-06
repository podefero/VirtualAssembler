#ifndef TRAPEXCEPTION_H
#define TRAPEXCEPTION_H

#include <stdexcept>

class TrapException : public std::runtime_error {
public:
  TrapException(const std::string &message) : std::runtime_error(message) {}
};

#endif
