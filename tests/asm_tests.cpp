#include "Assembly.h"
#include "PassOneException.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(AssemblyTest, ValidReadFile) {
  // Test if we can read file (get no error)
  Assembly assembly;
  std::string filePath = "we.asm";
  EXPECT_NO_THROW(assembly.readFile(filePath));
}

TEST(AssemblyTest, InvalidReadFile) {
  // Test if we get error when reading invalid file
  Assembly assembly;
  std::string filePath = "weInvalid.asm";
  EXPECT_THROW(assembly.readFile(filePath), PassOneException);
}

TEST(AssemblyTest, ValidStripComments) {
  // Test if we strip comments from buffer
  Assembly assembly;

  // Create a temporary file path
  std::string filePath = "temp_file.txt";

  // Create a file stream for writing
  std::ofstream outputFile(filePath);

  if (outputFile.is_open()) {
    std::string fileContents = ";This is a comment\n";
    outputFile << fileContents;
    outputFile.close();
  } else {
    FAIL() << "Failed to open the file for writing";
  }

  // Pass the file path to the readFile function
  EXPECT_NO_THROW(assembly.readFile(filePath));

  // strip comment
  std::string expect = "";
  assembly.stripComments();

  EXPECT_EQ(expect, assembly.getBuffer()[0]);
}

TEST(AssemblyTest, StripCommentsWithData) {
  // Test if we strip comments but not the valid data from buffer
  Assembly assembly;

  // Create a temporary file path
  std::string filePath = "temp_file.txt";

  // Create a file stream for writing
  std::ofstream outputFile(filePath);

  if (outputFile.is_open()) {
    std::string fileContents = "s        .BYT ';'   ;this is a comment\n";
    outputFile << fileContents;
    outputFile.close();
  } else {
    FAIL() << "Failed to open the file for writing";
  }

  // Pass the file path to the readFile function
  EXPECT_NO_THROW(assembly.readFile(filePath));

  // strip comment
  std::string expect = "s        .BYT ';'  ";
  assembly.stripComments();

  EXPECT_EQ(expect, assembly.getBuffer()[0]);
}

/*TEST(AssemblyTest, ReadAndStrip) {
  // More for me to debug not a test
  Assembly assembly;
  std::string filePath = "we.asm";
  EXPECT_NO_THROW(assembly.readFile(filePath));
  assembly.stripComments();
  std::vector<std::string> file = assembly.getBuffer();
  for (const std::string &line : file) {
    std::cout << line << std::endl;
  }
}*/

TEST(AssemblyTest, ValidReadTokenMultiSpace) {
  // Test if we can read a valid token that have tabs and spaces between
  Assembly assembly;
  std::string token_string = "W     .BYT      'W'";
  EXPECT_NO_THROW(assembly.readToken(token_string));
}

TEST(AssemblyTest, ValidReadTokenMultiSpaceAndCommas) {
  // Test if we can read a valid token that have spaces and commas
  Assembly assembly;
  std::string token_string = "ADD R1,     R2";
  EXPECT_NO_THROW(assembly.readToken(token_string));
}

TEST(AssemblyTest, InvalidReadTokenOperand1) {
  // Missing operand 1
  Assembly assembly;
  std::string token_string = "ADD ";
  EXPECT_THROW(assembly.readToken(token_string), PassOneException);
}

TEST(AssemblyTest, InvalidTokenByte) {
  // test a invalid token
  Assembly assembly;

  std::string line = "K.BYT 'K'";
  EXPECT_THROW(assembly.readToken(line), PassOneException);
}

TEST(AssemblyTest, InvalidTokenByteValue) {
  // test a valid token but invalid value
  Assembly assembly;

  std::string line = "K .BYT K"; // missing '' around K
  EXPECT_THROW(assembly.readToken(line), PassOneException);
}

TEST(AssemblyTest, InvalidTokenByteSize) {
  // test token size too large
  Assembly assembly;

  std::string line = "K .BYT 'K' p";
  EXPECT_THROW(assembly.readToken(line), PassOneException);
}

TEST(AssemblyTest, InvalidTokenSize) {
  // test token size too large
  Assembly assembly;

  std::string line = "K .BYT 'K' p t";
  EXPECT_THROW(assembly.readToken(line), PassOneException);
}

TEST(AssemblyTest, ValidTokenInstruct) {
  // Test instruction has the right bytes
  Assembly assembly;
  unsigned int expect_offset = 16;
  unsigned char bytes[] = {13, 0, 0, 0, 3, 0, 0, 0, 6, 0, 0, 0};

  std::string line = "ADD R3 R6";
  Token *token = assembly.readToken(line);

  std::vector<unsigned char> tokenBytes = token->getBytes();
  EXPECT_EQ(sizeof(bytes), tokenBytes.size());

  for (size_t i = 0; i < tokenBytes.size(); i++) {
    EXPECT_EQ(bytes[i], tokenBytes[i]);
  }
}

TEST(AssemblyTest, ValidTokenInt) {
  // check if we are getting correct byte size
  Assembly assembly;

  TokenInt token(5);
  EXPECT_EQ(4, token.getBytes().size());
}

TEST(AssemblyTest, ValidOffset) {
  // after reading a valid token updated offset
  Assembly assembly;
  unsigned int expect = 5;
  std::string line = "K .BYT 'K'";
  assembly.readToken(line);
  EXPECT_EQ(expect, assembly.offset);
}

TEST(AssemblyTest, ValidOffset2) {
  // after reading a valid token updated offset.
  // using two tokens
  Assembly assembly;
  unsigned int expect = 17;
  std::string line = "K .BYT 'K'";
  std::string line2 = "ADD R1 R2";
  // readinging in .byt so offset should be 5.
  assembly.readToken(line);
  assembly.readToken(line2);
  EXPECT_EQ(expect, assembly.offset);
}

TEST(AssemblyTest, ValidSymbolTable) {
  // build symbol table
  Assembly assembly;

  std::string line = "K .BYT 'K'";
  std::string line2 = "MAIN ADD R1 R2";
  // should have K at offset 5
  // MAIN at 17
  assembly.readToken(line);
  assembly.readToken(line2);

  unsigned int result1 = assembly.getSymbol("K");
  unsigned int result2 = assembly.getSymbol("MAIN");

  EXPECT_EQ(4, result1);
  EXPECT_EQ(5, result2);
}

TEST(AssemblyTest, InvalidSameSymbol) {
  // invalid symbol table. same symbol twice
  Assembly assembly;

  std::string line = "MAIN .BYT 'K'";
  std::string line2 = "MAIN ADD R1 R2";
  assembly.readToken(line);

  EXPECT_THROW(assembly.readToken(line2), PassOneException);
}

TEST(AssemblyTest, InvalidMissingSymbol) {
  // get a missing symbol
  Assembly assembly;

  std::string line = ".BYT 'K'";
  assembly.readToken(line);

  EXPECT_THROW(assembly.getSymbol("K"), PassOneException);
}

TEST(AssemblyTest, ValidDataSeg) {
  // check valid data segment
  Assembly assembly;

  std::string line = "K .BYT 'K'";  // 5
  std::string line2 = "T .BYT 'T'"; // 6
  std::string line3 = "ADD R2 R3";

  assembly.readToken(line);
  assembly.readToken(line2);
  assembly.readToken(line3);

  EXPECT_EQ(6, assembly.data_seg_end);
}

TEST(AssemblyTest, inValidDataSeg) {
  // check invalid data segment
  Assembly assembly;

  std::string line = "K .BYT 'K'";  // 5
  std::string line2 = "T .BYT 'T'"; // 6
  std::string line3 = "ADD R2 R3";  // 18
  std::string line4 = "L .BYT 'L'"; // 19 should throw here

  assembly.readToken(line);
  assembly.readToken(line2);
  assembly.readToken(line3);

  EXPECT_THROW(assembly.readToken(line4), PassOneException);
}

TEST(AssemblyTest, ValidImmediateValue) {
  // test immediate value func

  Assembly assembly;
  std::string min_value = "#-2147483648";
  std::string max_value = "#2147483647";
  int min_expect = -2147483648;
  int max_expect = 2147483647;

  std::string min_value_hex = "0x80000000";
  std::string max_value_hex = "0x7FFFFFFF";

  int min_result = assembly.getImmediate(min_value);
  int max_result = assembly.getImmediate(max_value);
  int min_value_hex_result = assembly.getImmediate(min_value_hex);
  int max_value_hex_result = assembly.getImmediate(max_value_hex);

  EXPECT_EQ(min_expect, min_result);
  EXPECT_EQ(max_expect, max_result);
  EXPECT_EQ(min_expect, min_value_hex_result);
  EXPECT_EQ(max_expect, max_value_hex_result);
}

TEST(AssemblyTest, InvalidImmediateValue) {
  // test immediate value func

  Assembly assembly;
  std::string min_value = "#-2147483649";
  std::string max_value = "#2147483648";

  std::string min_value_hex = "0x80000001";

  EXPECT_THROW(assembly.getImmediate(min_value), PassOneException);
  EXPECT_THROW(assembly.getImmediate(max_value), PassOneException);
  EXPECT_THROW(assembly.getImmediate(min_value_hex), PassOneException);
}

TEST(AssemblyTest, ValidPassOne) {
  Assembly assembly;

  // Create a temporary file path
  std::string filePath = "temp_file.txt";

  // Create a file stream for writing
  std::ofstream outputFile(filePath);

  if (outputFile.is_open()) {
    std::string fileContents =
        std::string("") + std::string(";This is a comment\n") +
        std::string("K .BYT 'K'\n") + std::string("ONE .INT #1\n") +
        std::string("TWO .INT 0x02\n") + std::string("\n") +
        std::string("ADD R1 R2\n") + std::string("TRP #0\n");
    outputFile << fileContents;
    outputFile.close();
  } else {
    FAIL() << "Failed to open the file for writing";
  }
  // tokens should be size 5
  // we should have one symbol K at offset 5
  EXPECT_NO_THROW(assembly.passOne(filePath));
  EXPECT_EQ(5, assembly.getTokens().size());
  EXPECT_EQ(4, assembly.getSymbol("K"));
}

TEST(AssemblyTest, InalidPass) {
  Assembly assembly;

  // Create a temporary file path
  std::string filePath = "temp_file.txt";

  // Create a file stream for writing
  std::ofstream outputFile(filePath);

  if (outputFile.is_open()) {
    std::string fileContents =
        std::string("  ADD  R1, R2\n") + std::string(" SUB   R2,R3\n") +
        std::string("DIV R3 R4\n") + std::string("STR R1 K\n") +
        std::string("STB R2   K\n") + std::string("MUL R2 R15\n") +
        std::string("MOV R6 R7\n") + std::string("LDR R8 TWO\n") +
        std::string("LDB R9 K\n");
    outputFile << fileContents;
    outputFile.close();
  } else {
    FAIL() << "Failed to open the file for writing";
  }

  // build tokens and table
  EXPECT_THROW(assembly.passOne(filePath), PassOneException);
  EXPECT_THROW(assembly.passTwo(), PassTwoException);
}

TEST(AssemblyTest, ValidPassTwoNoData) {
  Assembly assembly;

  // Create a temporary file path
  std::string filePath = "temp_file.txt";

  // Create a file stream for writing
  std::ofstream outputFile(filePath);

  if (outputFile.is_open()) {
    std::string fileContents =
        std::string("ADD R1 R2\n") + std::string("SUB R2 R3\n") +
        std::string("DIV R3 R4\n") + std::string("MUL R2 R15\n") +
        std::string("MOV R6 R7\n") + std::string("TRP #0\n");
    outputFile << fileContents;
    outputFile.close();
  } else {
    FAIL() << "Failed to open the file for writing";
  }

  // build tokens and table
  EXPECT_NO_THROW(assembly.passOne(filePath));
  EXPECT_NO_THROW(assembly.passTwo());
}

TEST(AssemblyTest, ValidPassTwo) {
  Assembly assembly;

  // Create a temporary file path
  std::string filePath = "temp_file.txt";

  // Create a file stream for writing
  std::ofstream outputFile(filePath);

  if (outputFile.is_open()) {
    std::string fileContents =
        std::string(";This is a comment\n") + std::string("K .BYT 'K'\n") +
        std::string("TWO .INT 0x02\n") + std::string("\n") +
        std::string("THREE .INT 0x03\n") + std::string(".BYT\n") +
        std::string("JMP MAIN\n") + std::string("MAIN ADD R1 R2\n") +
        std::string("SUB R2 R3\n") + std::string(" DIV R3 R4\n") +
        std::string(" DIV R4 R5\n") + std::string("STR R1 K\n") +
        std::string("STB R2 K\n") + std::string("MUL R2 R15\n") +
        std::string("MOV R6 R7\n") + std::string("LDR R8 TWO\n") +
        std::string("LDB R9 K\n") + std::string("TRP #0\n") +
        std::string("                                       ");
    outputFile << fileContents;
    outputFile.close();
  } else {
    FAIL() << "Failed to open the file for writing";
  }

  // build tokens and table
  EXPECT_NO_THROW(assembly.passOne(filePath));
  EXPECT_NO_THROW(assembly.passTwo());
  // EXPECT_EQ(42, assembly.bin_file.size());
}
