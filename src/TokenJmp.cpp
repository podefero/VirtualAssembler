#include "TokenJmp.h"

void TokenJmp::validate(std::map<std::string, unsigned int> &symbol_table,
                        unsigned int &limit) {
  unsigned int label_offset = symbol_table.at(label);
  // Needs to be after data seg
  if (label_offset > limit) {
    op1 = label_offset;
  } else
    throw PassTwoException("JMP label is in data seg. Should be in code seg.");
}
