#ifndef PASS_ONE_H
#define PASS_ONE_H

#include <stdexcept>

class PassOneException : public std::runtime_error {
public:
  PassOneException(const std::string &message) : std::runtime_error(message) {}
};

#endif
