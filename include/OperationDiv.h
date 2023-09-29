#ifndef OPERATION_DIV_H
#define OPERATION_DIV_H

#include "Operation.h"

class OperationDiv : public Operation {
public:
  OperationDiv(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
