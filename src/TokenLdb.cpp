#include "TokenLdb.h"

void TokenLdb::validate(std::map<std::string, unsigned int> &symbol_table,
                        unsigned int &limit) {
  unsigned int label_offset = symbol_table.at(label);
  // if label is in data seg
  if (label_offset <= limit) {
    op2 = label_offset;
  } else
    throw PassTwoException("LDB label is in code seg. Should be in data seg");
}
