#ifndef OPERATION_STB_H
#define OPERATION_STB_H

#include "Operation.h"

class OperationSTB : public Operation {
public:
  OperationSTB(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;

private:
  Memory memory;
};

#endif
