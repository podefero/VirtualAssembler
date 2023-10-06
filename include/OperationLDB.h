#ifndef OPERATION_LDB_H
#define OPERATION_LDB_H
#include "Operation.h"

class OperationLDB : public Operation {
public:
  OperationLDB(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
