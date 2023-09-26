#include "Directive.h"

class DirectiveInt : public Directive<int> {
public:
  DirectiveInt(unsigned int offset, int value) : offset(offset), value(value) {}
  unsigned int get_offset() override;
  int get_value() override;

private:
  unsigned int offset;
  int value;
};
