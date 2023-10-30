#include "TokenJmr.h"

void TokenJmr::validate(std::map<std::string, unsigned int> &symbol_table,
                        unsigned int &limit) {
  // Needs to be after data seg
  if (op1 < limit)
    throw PassTwoException("JMR RS " + std::to_string(op1) + " is in data seg. Should be in code seg.");
}
