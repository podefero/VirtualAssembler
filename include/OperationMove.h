#ifndef OPERATION_MOVE_H
#define OPERATION_MOVE_H

#include "Operation.h"

class OperationMove : public Operation {
public:
  OperationMove(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;

private:
  Memory memory;
};

#endif
