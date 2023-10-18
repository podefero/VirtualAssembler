#ifndef TOKEN_H
#define TOKEN_H

#include <map>
#include <string>
#include <vector>

class Token {
public:
  virtual ~Token() = default;
  virtual void validate(std::map<std::string, unsigned int> &symbol_table){};
  virtual std::vector<unsigned char> getBytes() { return {}; };
  unsigned int offset = 0;
};

#endif
