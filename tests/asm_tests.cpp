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

/*
TEST(AssemblyTest, ReadAndStrip) {
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

TEST(AssemblyTest, ValidTokenByte) {
  // test a valid tokey byte
  Assembly assembly;
  unsigned int expect_offset = 5;
  unsigned char expect_char = 'K';

  std::string line = "K .BYT 'K'";
  Token *token = assembly.readToken(line);

  EXPECT_EQ(expect_offset, token->offset);
  EXPECT_EQ(expect_char, token->getBytes()[0]);
}

TEST(AssemblyTest, ValidTokenByteNoValue) {
  // test a valid tokey byte
  Assembly assembly;
  unsigned int expect_offset = 5;
  unsigned char expect_char = '\0';

  std::string line = "K .BYT";
  Token *token = assembly.readToken(line);

  EXPECT_EQ(expect_offset, token->offset);
  EXPECT_EQ(expect_char, token->getBytes()[0]);
}

TEST(AssemblyTest, ValidTokenByteNoLabel) {
  // test a valid tokey byte
  Assembly assembly;
  unsigned int expect_offset = 5;
  unsigned char expect_char = 'K';

  std::string line = ".BYT K";
  Token *token = assembly.readToken(line);

  EXPECT_EQ(expect_offset, token->offset);
  EXPECT_EQ(expect_char, token->getBytes()[0]);
}

TEST(AssemblyTest, InvalidTokenByte) {
  // test a valid tokey byte
  Assembly assembly;

  std::string line = "K.BYT 'K'";
  EXPECT_THROW(assembly.readToken(line), PassOneException);
}

TEST(AssemblyTest, ValidOffset) {
  // after reading a valid token updated offset
  Assembly assembly;
  unsigned int expect = 5;
  std::string line = "K .BYT 'K'";
  // readinging in .byt so offset should be 5.
  assembly.readToken(line);
  EXPECT_EQ(expect, assembly.offset);
}
