#include "Assembly.h"
#include "PassOneException.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Assembly::Assembly() {}

// Try to read file and put it into buffer for future regex
// Throw execption if failed to read
void Assembly::readFile(const std::string &filePath) {
  std::ifstream fileStream(filePath);

  if (fileStream.is_open()) {
    std::string line;
    while (std::getline(fileStream, line)) {
      file_buffer.push_back(line);
    }
    fileStream.close();
  } else {
    throw(PassOneException("Failed to open asm file " + filePath));
  }
}

void Assembly::stripComments() {
  // first strip remove all ; but keep lines that have mix
  // of code and comments
  std::regex commentRegex("(^;.*)");
  for (std::string &line : file_buffer) {
    line = std::regex_replace(line, commentRegex, "");
  }

  // remove comments part of a line of code
  std::regex commentRegex2("( ;.*)");
  for (std::string &line : file_buffer) {
    line = std::regex_replace(line, commentRegex2, "");
  }
}

// read line to get token
Token *Assembly::readToken(std::string &line) {
  // split line into array
  std::vector<std::string> split;

  std::stringstream ss(line);
  std::string item;

  // when we push each item to the split we check if we have a valid token
  unsigned int index = 0;
  std::string tokenType = "NONE";

  // add items to the split array
  while (std::getline(ss, item, ' ')) {
    std::cout << item << std::endl;

    // set token type
    if (getTokenType(item) != "NONE") {
      tokenType = getTokenType(item);
    } else {
      index++;
    }

    split.push_back(item);
  }

  // if we didn't get a valid token after checking each item on the line
  if (tokenType == "NONE") {
    throw(PassOneException("No valid token found at " + line));
    return nullptr;
  }

  // based on size we can determine if we have label andor value/operands
  unsigned int size = split.size();

  // Make sure the tokens are in right positions
  if (size == 1) {
    return createToken(tokenType, "", "");
  } else if (size == 2) {
    // we could have a label on the left, or a value on the right
    if (index == 0) // value on right
      return createToken(tokenType, split[index + 1], "");
    if (index == 1) // label on left
      return createToken(tokenType, "", "");
  } else if (size == 3) {
    // if label is on left then we have one value
    // else we have two values
    if (index == 0) // no label, just values
      return createToken(tokenType, split[index + 1], split[index + 2]);
    if (index == 1) // we have a label, so one value
      return createToken(tokenType, split[index + 1], "");
  } else if (size == 4) {
    // we have a label, op1, and op2. Assuming they are in order
    if (index == 1) // still check token position
      return createToken(tokenType, split[index + 1], split[index + 2]);
  } else {
    // invalid token size
    throw(PassOneException("Invalid number of tokens: " + std::to_string(size) +
                           " should be between 1-4 in line " + line));
    return nullptr;
  }

  // if we get to hear then our tokens were not ordered right
  throw(PassOneException("Tokens out of order " + tokenType +
                         " was in potition " + std::to_string(index) +
                         " in line " + line));
  return nullptr;
}

// create token. value can be op1 for instructions
Token *Assembly::createToken(std::string tokenType, std::string value,
                             std::string op2) {
  if (tokenType == "BYTE") {
    offset += sizeof(unsigned char);
    return new TokenByte(offset, value[0]);
  } else {
    return nullptr;
  }
}

// Checks if the item passed in can match a directive or instruction
std::string Assembly::getTokenType(std::string &item) {
  std::string instruction = "INST";
  if (item == ".BYT") {
    return "BYTE";
  } else if (item == ".INT") {
    return "INT";
  } else if (item == ".STR") {
    return "STR";
  } else if (item == "ADD") {
    return instruction;
  } else if (item == "DIV") {
    return instruction;
  } else if (item == "MUL") {
    return instruction;
  } else if (item == "MOV") {
    return instruction;
  } else if (item == "STR") {
    return instruction;
  } else if (item == "LDR") {
    return instruction;
  } else if (item == "STB") {
    return instruction;
  } else if (item == "LDB") {
    return instruction;
  } else if (item == "TRP") {
    return instruction;
  } else {
    return "NONE";
  }
}

std::vector<std::string> Assembly::getBuffer() { return file_buffer; }
