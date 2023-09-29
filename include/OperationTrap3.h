#ifndef OPERATION_TRAP_3_H
#define OPERATION_TRAP_3_H

#include "Operation.h"

class OperationTrap3 : public Operation {
public:
  OperationTrap3(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
