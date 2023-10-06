#ifndef OPERATION_JUMP_H
#define OPERATION_JUMP_H

#include "Operation.h"

class OperationJump : public Operation {
public:
  OperationJump(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
