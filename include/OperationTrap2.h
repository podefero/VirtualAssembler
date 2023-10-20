#ifndef OPERATION_TRAP_2_H
#define OPERATION_TRAP_2_H

#include "Operation.h"

class OperationTrap2 : public Operation {
public:
  OperationTrap2(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
