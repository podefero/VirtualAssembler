#include "Assembly.h"
#include "PassOneException.h"
#include "PassTwoException.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

Assembly::Assembly() {}

// Try to read file and put it into buffer for future regex
// Throw exception if failed to read
void Assembly::readFile(const std::string &filePath) {
    std::ifstream fileStream(filePath);

    if (fileStream.is_open()) {
        std::string line;
        while (std::getline(fileStream, line)) {
            file_buffer.push_back(line);
        }
        fileStream.close();
    } else {
        throw (PassOneException("Failed to open asm file " + filePath));
    }
}

// Reads in file, strips comments, reads tokens and builds symbol_table
void Assembly::passOne(const std::string &filePath) {
    outputFile = filePath;
    outputFile[outputFile.size() - 1] = 'n';
    outputFile[outputFile.size() - 2] = 'i';
    outputFile[outputFile.size() - 3] = 'b';
    size_t count = 0;
    size_t size = 0;
    try {
        readFile(filePath);
        stripComments();
        for (const std::string &line: file_buffer) {
            // read in line, if empty skip. Else tokenize
            if (!line.empty()) {
                count++;
                std::string copy_line = line;
                size = copy_line.size();
                // tokenize
                Token *token = readToken(copy_line);
                if (token) {
                    tokens.push_back(token);
                    // std::string hex_line = toHex(token->getBytes());
                    //  std::cout << std::to_string(count) << " " << line << std::endl;
                }
                if (found_trap0)
                    return;
            }
        }
        // readToken function will build symbol_table
    }

    catch (const PassOneException &ex) {
        dumpSymbolTable();
        throw PassOneException(std::string(ex.what()) +
                               " line #: " + std::to_string(count) +
                               " size: " + std::to_string(size));
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
            // std::cout << std::to_string(i + 1) << " " <<
            // toHex(tokens[i]->getBytes())
            //<< std::endl;
        } catch (const PassTwoException &ex) {
            dumpSymbolTable();
            throw PassTwoException(std::string(ex.what()) + " at index " +
                                   std::to_string(i) + " in tokens");
            return;
        } catch (const std::out_of_range &e) {
            dumpSymbolTable();
            throw PassTwoException(
                    "Key not found in the symbol table during pass two " +
                    toHex(tokens[i]->getBytes()) + " index:" + std::to_string(i) +
                    " symbol: " + tokens[i]->label);
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
    std::ofstream file(outputFile, std::ios::binary);

    // Check if the file was successfully opened
    if (file.is_open()) {
        // Write the contents of the vector to the file
        file.write(reinterpret_cast<const char *>(bin_file.data()),
                   bin_file.size());

        // Close the file
        file.close();
    } else {
        throw (PassTwoException("Failed to write bin file"));
    }
}

void Assembly::stripComments() {
    std::regex whiteSpace("^\\s+");
    std::regex commentRegex("(^;.*)|( ;.*)|(\\s+$)");
    for (std::string &line: file_buffer) {
        line = std::regex_replace(line, whiteSpace, "");
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
    std::regex fspaceRegex("^\\s+"); // remove first space in line
    std::regex emptyQuote("'\\s+'");

    line = std::regex_replace(line, commaRegex, " ");
    line = std::regex_replace(line, spaceRegex, " ");
    line = std::regex_replace(line, fspaceRegex, "");
    line = std::regex_replace(line, emptyQuote, "'\\0'");

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
        throw (PassOneException("Invalid number of tokens: " + std::to_string(size) +
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
            std::string tokenType;
            tokenType = split[index];
            //std::cout << size << std::endl;
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
                return token;
            } else if (size == 1) {
                throw PassOneException("Bad token ");
            }
        }
    } // Failed to create token, catch and wrap into a new throw
    catch (const PassOneException &ex) {
        throw (PassOneException(std::string(ex.what()) + " at line " + line));
    }

    // if we get to here then our tokens were not ordered right
    throw (PassOneException("Tokens out of order or invalid.\nLine: " + line
                            + "\nSize: " + std::to_string(size)));
    return nullptr;
}

// create token. arg1 can be op1 for instructions
Token *Assembly::createToken(const std::string &item, const std::string &arg1,
                             const std::string &arg2) {
    unsigned int instr_size = 12;
    Token *token = nullptr;

    // Check each token and see if we can match a valid one
    if (item == ".BYT" && !done_instruction) {

        //.BYT
        offset += 1;
        if (!arg1.empty()) {
            token = new TokenByte(getImmediate(arg1));
        } else
            token = new TokenByte('\0');

    } else if (item == ".INT") {

        //.INT
        offset += 4;
        int immediate = 0;
        if (!arg1.empty())
            immediate = getImmediate(arg1);
        token = new TokenInt(immediate);

    } else if (item == ".STR") {
        return nullptr;
    } else if (item == "ADD") {

        // ADD
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rs = getValidRegister(arg2);
        token = new TokenInstr(rd, rs, OpCode::ADD);

    } else if (item == "ADDI") {

        // ADD
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int immediate = getImmediate(arg2);
        token = new TokenInstr(rd, immediate, OpCode::ADDI);

    } else if (item == "SUB") {

        // SUB
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rs = getValidRegister(arg2);
        token = new TokenInstr(rd, rs, OpCode::SUB);

    } else if (item == "DIV") {

        // DIV
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rs = getValidRegister(arg2);
        token = new TokenInstr(rd, rs, OpCode::DIV);

    } else if (item == "MUL") {

        // MUL
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rs = getValidRegister(arg2);
        token = new TokenInstr(rd, rs, OpCode::MUL);

    } else if (item == "MOV") {

        // MOV
        offset += instr_size;
        int rd = getSpecialRegister(arg1);
        int rs = getSpecialRegister(arg2);
        token = new TokenInstr(rd, rs, OpCode::MOV);

    } else if (item == "MOVI") {

        //MOVI
        offset += instr_size;
        int rd = getValidRegister(arg1);
        token = new TokenInstr(rd, getImmediate(arg2), OpCode::MOVI);

    } else if (item == "JMP") {

        // JMP
        offset += instr_size;
        token = new TokenInstr(0, 0, OpCode::JMP);
        token->label = arg1;

    } else if (item == "JMR") {

        //JMR
        offset += instr_size;
        token = new TokenInstr(getValidRegister(arg1), 0, OpCode::JMR);

    } else if (item == "BNZ") {

        //BNZ
        offset += instr_size;
        token = new TokenInstr(getValidRegister(arg1), 0, OpCode::BNZ);
        token->label = arg2;

    } else if (item == "BGT") {

        //BGT
        offset += instr_size;
        token = new TokenInstr(getValidRegister(arg1), 0, OpCode::BGT);
        token->label = arg2;

    } else if (item == "BLT") {

        //BLT
        offset += instr_size;
        token = new TokenInstr(getValidRegister(arg1), 0, OpCode::BLT);
        token->label = arg2;

    } else if (item == "BRZ") {

        //BRZ
        offset += instr_size;
        token = new TokenInstr(getValidRegister(arg1), 0, OpCode::BRZ);
        token->label = arg2;

    } else if (item == "STR") {

        // STR
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rg;
        try {
            rg = getValidRegister(arg2);
            token = new TokenInstr(rd, rg, OpCode::STRI);
        } catch (const PassOneException &ex) {
            token = new TokenInstr(rd, 0, OpCode::STR);
            token->label = arg2;
            token->inDataSeg = true;
        }
    } else if (item == "LDR") {

        // LDR or LDRI (indirect)
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rg;
        try {
            // if we have an valid rg then we use LDB RD RG, else LDB RD Label
            rg = getValidRegister(arg2);
            token = new TokenInstr(rd, rg, OpCode::LDRI);
        } catch (const PassOneException &ex) {
            token = new TokenInstr(rd, 0, OpCode::LDR);
            token->label = arg2;
            token->inDataSeg = true;
        }
    } else if (item == "LDA") {

        //LDA
        offset += instr_size;
        token = new TokenInstr(getValidRegister(arg1), 0, OpCode::LDA);
        token->label = arg2;
        token->inDataSeg = true;

    } else if (item == "STB") {

        // STB or STBI (indirect)
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rg;
        try {
            // if we have an valid rg then we use LDB RD RG, else LDB RD Label
            rg = getValidRegister(arg2);
            token = new TokenInstr(rd, rg, OpCode::STBI);
        } catch (const PassOneException &ex) {
            token = new TokenInstr(rd, 0, OpCode::STB);
            token->label = arg2;
            token->inDataSeg = true;
        }
    } else if (item == "LDB") {

        // LDB or LDBI (indirect)
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rg;
        try {
            // if we have an valid rg then we use LDB RD RG, else LDB RD Label
            rg = getValidRegister(arg2);
            token = new TokenInstr(rd, rg, OpCode::LDBI);
        } catch (const PassOneException &ex) {
            token = new TokenInstr(rd, 0, OpCode::LDB);
            token->label = arg2;
            token->inDataSeg = true;
        }
    } else if (item == "CMP") {

        //CMP
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rs = getValidRegister(arg2);
        token = new TokenInstr(rd, rs, OpCode::CMP);

    } else if (item == "CMPI") {

        //CMP
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int immediate = getImmediate(arg2);
        token = new TokenInstr(rd, immediate, OpCode::CMPI);

    } else if (item == "AND" || item == "OR" || item == "NOT") {

        //AND,OR,NOT
        offset += instr_size;
        int rd = getValidRegister(arg1);
        int rs = getValidRegister(arg2);
        OpCode opCode;
        if (item == "AND")
            opCode = OpCode::AND;
        else if (item == "OR")
            opCode = OpCode::OR;
        else
            opCode = OpCode::NOT;
        token = new TokenInstr(rd, rs, opCode);

    } else if (item == "PUSH" || item == "POP") {

        offset += instr_size;
        //PUSH POP
        int rg = getValidRegister(arg1);
        OpCode opCode;
        if (item == "PUSH") {
            opCode = OpCode::PUSH;
        } else {
            opCode = OpCode::POP;
        }
        token = new TokenInstr(rg, 0, opCode);

    } else if (item == "TRP") {

        // TRP
        offset += instr_size;

        //  get the immediate arg1
        //  based on that arg1 return the right trap
        int immediate = getImmediate(arg1);
        // trap 0
        if (immediate == 0) {
            found_trap0 = true;
            token = new TokenInstr(immediate, 0, OpCode::TRAP);
        }
            // set token if the trap is a valid arg1
        else if (immediate > 0 && immediate <= 7)
            token = new TokenInstr(immediate, 0, OpCode::TRAP);
        else
            throw PassOneException("Invalid TRP arg1 " + std::to_string(immediate));
    }


    // if we have a valid token and if it's not a directive
    // then set the done_instruction flag
    // and set code_seg.
    if (token && item != ".BYT" && item != ".STR" && item != ".INT") {
        if (!done_instruction) { // so it only sets once
            done_instruction = true;
            //- instr size becuase we moved offset from read inst
            //-1 becuase data seg is back one
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
        if (registerNumber >= 0 && registerNumber <= 15) {
            return (registerNumber);
        }
    }

    throw PassOneException("Invalid Register value " +
                           std::to_string(registerNumber));
    return -1;
}

//same as getValidRegister but includes the others
int Assembly::getSpecialRegister(const std::string &item) {
    int registerNumber = 0;
    if (item[0] == 'R') {
        registerNumber = std::stoi(item.substr(1));
        if (registerNumber >= 0 && registerNumber <= 21) {
            return (registerNumber);
        }
    }

    throw PassOneException("Invalid Special Register value " +
                           std::to_string(registerNumber));
    return -1;
}

// get immediate value
// support # or 0x
// support ''
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
        } else if (item[0] == '\'') {

            // check if it's a valid unsigned char
            if (item.length() == 3) {
                return static_cast<unsigned char>(item[1]);
            } else if (item.length() == 4) {
                //handle special characters
                if (item == "'\\n'") {
                    // Handle newline character
                    return static_cast<unsigned char>('\n');
                } else if (item == "'\\t'") {
                    // Handle tab character
                    return static_cast<unsigned char>('\t');
                } else if (item == "'\\0'") {
                    //empty space
                    return static_cast<unsigned char>(' ');
                } else {
                    throw PassOneException("getImmediate(). Special symbol no supported " + item);
                }

            }
        } else {
            throw PassOneException("getImmediate() failed to get value from item: " +
                                   item);
        }

    } catch (const std::out_of_range &e) {
        throw PassOneException("getImmediate() out of range: " + item);
    } catch (const std::invalid_argument &e) {
        throw PassOneException("getImmediate() Invalid argument: " + item);
    }
    return 0;
}

// access value from symbol table
unsigned int Assembly::getSymbol(const std::string &key) {
    try {
        return symbol_table.at(key);
    } catch (const std::out_of_range &e) {
        throw PassOneException("Key: " + key + " not found in the symbol table");
        return 0;
    }
}

// convert char array to hex
std::string Assembly::toHex(const std::vector<unsigned char> &data) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c: data) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    return ss.str();
}

void Assembly::dumpSymbolTable() {
    std::cerr << "Symbol Table " << std::endl;
    std::cerr << "***" << std::endl;
    for (const auto &item: symbol_table) {
        std::cerr << item.first << ": " << item.second << std::endl;
    }
    std::cerr << "***" << std::endl;
}

const std::vector<std::string> &Assembly::getBuffer() { return file_buffer; }

std::vector<Token *> Assembly::getTokens() { return tokens; }
