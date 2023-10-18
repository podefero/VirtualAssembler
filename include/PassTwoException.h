#ifndef PASS_TWO_H
#define PASS_TWO_H

#include <stdexcept>

class PassTwoException : public std::runtime_error {
public:
  PassTwoException(const std::string &message) : std::runtime_error(message) {}
};

#endif
