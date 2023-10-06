#ifndef OPERATION_H
#define OPERATION_H

#include "Memory.h"
#include "Registers.h"

class Operation {
public:
  virtual ~Operation() = default;
  virtual int validate(Memory &memory) = 0;
  virtual int execute(Memory &memory) = 0;

protected:
  int operand1 = 0;
  int operand2 = 0;
};

#endif
