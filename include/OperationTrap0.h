#ifndef OPERATION_TRAP_0_H
#define OPERATION_TRAP_0_H

#include "Operation.h"

class OperationTrap0 : public Operation {
public:
  OperationTrap0(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
