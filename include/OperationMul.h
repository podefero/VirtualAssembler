#ifndef OPERATION_MUL_H
#define OPERATION_MUL_H

#include "Operation.h"

class OperationMul : public Operation {
public:
  OperationMul(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
