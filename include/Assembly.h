#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include "Tokens.h"
#include <map>
#include <regex>
#include <string>
#include <vector>

class Assembly {
public:
    Assembly();

    void readFile(const std::string &filePath);

    void passOne(const std::string &filePath);

    void passTwo();

    void stripComments();

    const std::vector<std::string> &getBuffer();

    Token *readToken(std::string &line);

    Token *createToken(const std::string& tokenType, const std::string& arg1,
                        const std::string& arg2);

    static int getValidRegister(const std::string &item);

    static int getImmediate(const std::string &item);

    unsigned int getSymbol(const std::string& key);

    void dumpSymbolTable();

    std::vector<Token *> getTokens();

    unsigned int offset = 4; // pc is 4 bytes t
    unsigned int data_seg_end{};
    std::vector<unsigned char> bin_file;
    std::map<std::string, unsigned int> symbol_table;

    static std::string toHex(const std::vector<unsigned char> &data);

private:
    std::vector<std::string> file_buffer;
    std::vector<Token *> tokens;
    bool done_instruction = false;
    bool found_trap0 = false;
    std::string outputFile;
};

#endif
