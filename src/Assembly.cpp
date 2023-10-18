#include "Assembly.h"
#include "PassOneException.h"
#include "PassTwoException.h"
#include <fstream>
#include <iostream>
#include <sstream>

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

// Reads in file, strips comments, reads tokens and builds symbol_table
void Assembly::passOne(const std::string &filePath) {
  try {
    readFile(filePath);
    stripComments();
    for (size_t i = 0; i < file_buffer.size(); i++) {
      // read in line, if empty skip. Else tokenize
      std::string line = file_buffer[i];
      if (line.empty())
        continue;

      // tokenize
      // readToken function will build symbol_table
      Token *token = readToken(line);
      if (token) {
        tokens.push_back(token);
      }
    }
  } catch (const PassOneException &ex) {
    throw PassOneException(std::string(ex.what()));
  }
}

// build binary file from symbol table and tokens
void Assembly::passTwo() {
  for (size_t i = 0; i < tokens.size(); i++) {
    // validate
    try {
      tokens[i]->validate(symbol_table);
    } catch (const PassTwoException &ex) {
      throw PassTwoException(std::string(ex.what()) + " at index " +
                             std::to_string(i) + " in tokens");
      return;
    }

    // write to file
    for (size_t j = 0; j < tokens[i]->getBytes().size(); j++) {
      bin_file.push_back(tokens[i]->getBytes()[j]);
    }

    // finished with token
    delete tokens[i];
  }

  // write file
  //  Open the file in binary mode for writing
  std::ofstream file("output.bin", std::ios::binary);

  // Check if the file was successfully opened
  if (file.is_open()) {
    // Write the contents of the vector to the file
    file.write(reinterpret_cast<const char *>(bin_file.data()),
               bin_file.size());

    // Close the file
    file.close();
  } else {
    throw(PassTwoException("Failed to write bin file"));
  }
}

void Assembly::stripComments() {
  std::regex commentRegex("(^;.*)|( ;.*)");
  for (std::string &line : file_buffer) {
    line = std::regex_replace(line, commentRegex, "");
  }
}

// read line to get token
Token *Assembly::readToken(std::string &line) {
  // split line into array
  std::vector<std::string> split;

  // before spliting remove commas and  extra white space
  std::regex commaRegex(",");
  std::regex spaceRegex("\\s+");

  line = std::regex_replace(line, commaRegex, " ");
  line = std::regex_replace(line, spaceRegex, " ");

  std::stringstream ss(line);
  std::string item;

  // add items to the split array
  while (std::getline(ss, item, ' ')) {
    // std::cout << item << std::endl;

    split.push_back(item);
  }

  // based on size we can determine if we have label andor value/operands
  unsigned int size = split.size();

  // Make sure the tokens are in right positions
  if (size == 0 || size > 4) {
    // invalid token size
    throw(PassOneException("Invalid number of tokens: " + std::to_string(size) +
                           ".. Should be between 1-4 in line " + line));
    return nullptr;
  }

  // the createToken will check if instruction has the required operand
  try {
    // if index = 1 then we have a label on left to put in symbol table.
    for (unsigned int index = 0; index < 2; index++) {
      Token *token;
      std::string tokenType = split[index];
      if (size == 1) {
        token = createToken(tokenType, "", "");
      } else if (size == 2) {
        // we could have a label on the left, or a value on the right
        if (index == 0) {
          token = createToken(tokenType, split[index + 1], "");
        }
        if (index == 1) // label on left
          token = createToken(tokenType, "", "");
      } else if (size == 3) {
        // if label is on left then we have one value
        // else we have two values
        if (index == 0) // no label, just values
          token = createToken(tokenType, split[index + 1], split[index + 2]);
        if (index == 1) // we have a label, so one value
          token = createToken(tokenType, split[index + 1], "");
      } else if (size == 4) {
        if (index == 0)
          token = nullptr;
        // we have a label, op1, and op2. Assuming they are in order
        if (index == 1) // still check token position
          token = createToken(tokenType, split[index + 1], split[index + 2]);
      }
      if (token != nullptr) {
        // add symbol to table
        if (index == 1) {
          if (symbol_table.count(split[0]) == 0)
            symbol_table.emplace(split[0], offset);
          else
            throw PassOneException("Symbol: " + split[0] +
                                   " already exist in table. line " + line);
        }
        return token;
      }
    }
  } // Failed to create token, catch and wrap into a new throw
  catch (const PassOneException &ex) {
    throw(PassOneException(std::string(ex.what()) + " at line " + line));
  }

  // if we get to here then our tokens were not ordered right
  throw(PassOneException("Tokens out of order or invalid " + line));
  return nullptr;
}

// create token. value can be op1 for instructions
Token *Assembly::createToken(const std::string item, const std::string value,
                             const std::string op2) {
  unsigned int instr_size = 12;
  Token *token = nullptr;
  if (item == ".BYT" && !done_instruction) {
    offset += 1;
    if (op2.size() != 0)
      throw(
          PassOneException("Invalid .BYTE, should not have second parameter "));
    // size for a valid char in assembly, 'W'
    else if (value.size() == 3)
      token = new TokenByte(offset, value[1]);
    else if (value.size() == 0) {
      token = new TokenByte(offset, '\0');
    } else
      throw(PassOneException("Invalid .BYTE value: " + value));
  } /*else if (item == ".INT") {
    return nullptr;
  } else if (item == ".STR") {
    return nullptr;
  }*/
  else if (item == "ADD") {
    offset += instr_size;
    // check if op1 and op2 are valid registers
    int rd = getValidRegister(value);
    int rs = getValidRegister(op2);
    if (rd == -1 || rs == -1) {
      throw(
          PassOneException("Invalid register(s) RD: " + value + " RS: " + op2));
    } else
      token = new TokenAdd(offset, rd, rs);
  } /* else if (item == "DIV") {
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
  }*/

  // if we have a valid token and if it's not a directive
  // then set the done_instruction flag
  // and set code_seg.
  if (token && item != ".BYT" && item != ".STR" && item != ".INT") {
    if (!done_instruction) { // so it only sets once
      done_instruction = true;
      data_seg_end = (offset - instr_size);
      // write this to binary file
      bin_file.push_back(static_cast<unsigned char>(data_seg_end & 0xFF));
      bin_file.push_back(
          static_cast<unsigned char>((data_seg_end >> 8) & 0xFF));
      bin_file.push_back(
          static_cast<unsigned char>((data_seg_end >> 16) & 0xFF));
      bin_file.push_back(
          static_cast<unsigned char>((data_seg_end >> 24) & 0xFF));
    }
  }
  if (token && (item == ".BYTE" || item == ".STR" || item == ".INT")) {
    throw PassOneException("Can't use Directive: " + item +
                           " past data segment");
  }
  return token;
}

// if it's a valid register return register number
int Assembly::getValidRegister(const std::string &item) {
  if (item.size() == 2) {
    if (item[0] == 'R') {
      int registerNumber = std::stoi(item.substr(1));
      // check if in register range 0-15
      if (registerNumber >= 0 && registerNumber <= 15) {
        return static_cast<unsigned int>(registerNumber);
      }
    }
  }

  return -1;
}

// access value from symbol table
unsigned int Assembly::getSymbol(const std::string key) {
  try {
    return symbol_table.at(key);
  } catch (const std::out_of_range &e) {
    throw PassOneException("Key: " + key + " not found in the symbol table");
    return 0;
  }
}
const std::vector<std::string> &Assembly::getBuffer() { return file_buffer; }
std::vector<Token *> Assembly::getTokens() { return tokens; }
