#ifndef OPERATION_STR_H
#define OPERATION_STR_H

#include "Operation.h"

class OperationSTR : public Operation {
public:
  OperationSTR(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;

private:
  Memory memory;
};

#endif
