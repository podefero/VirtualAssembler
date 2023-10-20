#ifndef OPERATION_TRAP_4_H
#define OPERATION_TRAP_4_H

#include "Operation.h"

class OperationTrap4 : public Operation {
public:
  OperationTrap4(int op1, int op2);

  int validate(Memory &memory) override;
  int execute(Memory &memory) override;
};

#endif
