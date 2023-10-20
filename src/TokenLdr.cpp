#include "TokenLdr.h"
void TokenLdr::validate(std::map<std::string, unsigned int> &symbol_table,
                        unsigned int &limit) {
  unsigned int label_offset = symbol_table.at(label);
  // if label is in data seg
  if (label_offset <= limit) {
    op2 = label_offset;
  } else
    throw PassTwoException("LDR label is in code seg. Should be in data seg");
}
