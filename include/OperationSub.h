#ifndef OPERATION_SUB_H
#define OPERATION_SUB_H

#include "Operation.h"

class OperationSub : public Operation {
public:
  OperationSub(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
