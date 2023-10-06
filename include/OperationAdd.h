#ifndef OPERATION_ADD_H
#define OPERATION_ADD_H

#include "Operation.h"

class OperationAdd : public Operation {
public:
  OperationAdd(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
