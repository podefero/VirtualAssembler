#ifndef OPERATION_EXCEPTION_H
#define OPERATION_EXCEPTION_H

#include <stdexcept>

class OperationException : public std::runtime_error {
public:
  OperationException(const std::string &message)
      : std::runtime_error(message) {}
};

#endif
