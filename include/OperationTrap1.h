#ifndef OPERATION_TRAP_1_H
#define OPERATION_TRAP_1_H

#include "Operation.h"

class OperationTrap1 : public Operation {
public:
  OperationTrap1(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
