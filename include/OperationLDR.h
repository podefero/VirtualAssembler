#ifndef OPERATION_LDR_H
#define OPERATION_LDR_H

#include "Operation.h"

class OperationLDR : public Operation {
public:
  OperationLDR(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
