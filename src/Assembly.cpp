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
    for (const std::string &line : file_buffer) {
      // read in line, if empty skip. Else tokenize
      if (line.size() != 0) {
        std::string copy_line = line;
        // tokenize
        Token *token = readToken(copy_line);
        if (token)
          tokens.push_back(token);
      }
    }
    // readToken function will build symbol_table
  }

  catch (const PassOneException &ex) {
    throw PassOneException(std::string(ex.what()));
  }

  // if we haven't found trap 0 then throw
  if (!found_trap0) {
    throw PassOneException("No trap 0 found");
  }
}

// build binary file from symbol table and tokens
void Assembly::passTwo() {
  for (size_t i = 0; i < tokens.size(); i++) {
    // validate
    try {
      tokens[i]->validate(symbol_table, data_seg_end);
    } catch (const PassTwoException &ex) {
      throw PassTwoException(std::string(ex.what()) + " at index " +
                             std::to_string(i) + " in tokens");
      return;
    } catch (const std::out_of_range &e) {
      throw PassTwoException(
          "Key not found in the symbol table during pass two " +
          std::string(e.what()));
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
  std::regex commentRegex("(^;.*)|( ;.*)|(\\s$)");
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
      // build symbol table
      if (index == 1) {
        // check if we do have a key in the table
        if (symbol_table.count(split[0]) == 0)
          symbol_table.emplace(split[0], offset);
        else
          throw PassOneException("Symbol: " + split[0] +
                                 " already exist in table. line " + line);
      }

      Token *token = nullptr;
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
        // index 1 means we do have a label on the left
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

  // Check each token and see if we can match a valid one
  if (item == ".BYT" && !done_instruction) {
    offset += 1;

    // if we have a second param throw.
    if (op2.size() != 0)
      throw(
          PassOneException("Invalid .BYTE, should not have second parameter "));

    // size for a valid char in assembly, 'W'
    else if (value.size() == 3)
      token = new TokenByte(value[1]);
    else if (value.size() == 0) {
      token = new TokenByte('\0');
    } else
      throw(PassOneException("Invalid .BYTE value: " + value));
  } else if (item == ".INT") {

    //.INT
    offset += 4;
    int immediate = 0;
    if (value.size() > 0)
      immediate = getImmediate(value);
    token = new TokenInt(immediate);

  } else if (item == ".STR") {
    return nullptr;
  } else if (item == "ADD") {

    // ADD
    offset += instr_size;
    int rd = getValidRegister(value);
    int rs = getValidRegister(op2);
    token = new TokenAdd(rd, rs);

  } else if (item == "SUB") {

    // SUB
    offset += instr_size;
    int rd = getValidRegister(value);
    int rs = getValidRegister(op2);
    token = new TokenSub(rd, rs);

  } else if (item == "DIV") {

    // DIV
    offset += instr_size;
    int rd = getValidRegister(value);
    int rs = getValidRegister(op2);
    token = new TokenDiv(rd, rs);

  } else if (item == "MUL") {

    // MUL
    offset += instr_size;
    int rd = getValidRegister(value);
    int rs = getValidRegister(op2);
    token = new TokenMul(rd, rs);

  } else if (item == "MOV") {

    // MOV
    offset += instr_size;
    int rd = getValidRegister(value);
    int rs = getValidRegister(op2);
    token = new TokenMove(rd, rs);

  } else if (item == "JMP") {

    // JMP
    TokenJmp *token_jmp;
    offset += instr_size;
    token_jmp = new TokenJmp(0, 0);
    token_jmp->label = value;
    token = token_jmp;

  } else if (item == "STR") {

    // STR
    // Use TokenStr ptr to handle setting the label variable
    TokenStr *token_str;
    offset += instr_size;
    int rs = getValidRegister(value);
    token_str = new TokenStr(rs, 0);
    token_str->label = op2;
    token = token_str;

  } else if (item == "LDR") {

    // LDR
    TokenLdr *token_ldr;
    offset += instr_size;
    int rd = getValidRegister(value);
    token_ldr = new TokenLdr(rd, 0);
    token_ldr->label = op2;
    token = token_ldr;

  } else if (item == "STB") {

    // STB
    TokenStb *token_stb;
    offset += instr_size;
    int rs = getValidRegister(value);
    token_stb = new TokenStb(rs, 0);
    token_stb->label = op2;
    token = token_stb;

  } else if (item == "LDB") {

    // LDB
    TokenLdb *token_ldb;
    offset += instr_size;
    int rd = getValidRegister(value);
    token_ldb = new TokenLdb(rd, 0);
    token_ldb->label = op2;
    token = token_ldb;

  } else if (item == "TRP") {
    // get the immediate value
    // based on that value return the right trap
    int immediate = getImmediate(value);
    // trap 0
    if (immediate == 0) {
      found_trap0 = true;
      token = new TokenTrap(immediate, 0);
    }
    // set token if the trap is a valid value
    else if (immediate > 0 && immediate <= 7)
      token = new TokenTrap(immediate, 0);
    else
      throw PassOneException("Invalid TRP value " + immediate);
  }

  // if we have a valid token and if it's not a directive
  // then set the done_instruction flag
  // and set code_seg.
  if (token && item != ".BYT" && item != ".STR" && item != ".INT") {
    if (!done_instruction) { // so it only sets once
      done_instruction = true;
      //- instr size becuase we moved offset from read inst
      //-1 becuase data seg is back one
      data_seg_end = (offset - instr_size) - 1;
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
  if (token && done_instruction &&
      (item == ".BYTE" || item == ".STR" || item == ".INT")) {
    throw PassOneException("Can't use Directive: " + item +
                           " past data segment");
  }
  return token;
}

// if it's a valid register return register number
int Assembly::getValidRegister(const std::string &item) {
  int registerNumber = 0;
  if (item[0] == 'R') {
    registerNumber = std::stoi(item.substr(1));
    // check if in register range 0-15
    if (registerNumber >= 0 && registerNumber <= 15) {
      return static_cast<unsigned int>(registerNumber);
    }
  }

  throw PassOneException("Invalid Register value " +
                         std::to_string(registerNumber));
  return -1;
}

// get immediate value
// support # or 0x
int Assembly::getImmediate(const std::string &item) {
  try {
    if (item[0] == '#') {
      int value = std::stoi(item.substr(1));

      return value;
    } else if (item[0] == '0' && item[1] == 'x') {
      // could not get this to convert with stoi so manually checking it
      if (item == "0x80000000")
        return -2147483648;
      int value = std::stoi(item.substr(2), nullptr, 16);
      return value;
    }

  } catch (const std::out_of_range &e) {
    throw PassOneException("getImmediate() out of range: " + item);
  } catch (const std::invalid_argument &e) {
    throw PassOneException("getImmediate() Invalid argument: " + item);
  }
  return 0;
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
