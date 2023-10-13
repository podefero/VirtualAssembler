#ifndef TOKEN_H
#define TOKEN_H

#include <vector>

class Token {
public:
  virtual ~Token() = default;
  virtual void validate(unsigned int &offset){};
  virtual std::vector<unsigned char> getBytes() { return {}; };
};

#endif
