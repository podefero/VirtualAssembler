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

TEST(AssemblyTest, ValidReadToken) {
  // Test if we can read a valid token
  Assembly assembly;
  std::string token_string = "W .BYT 'W'";
  Token *token = assembly.readToken(token_string);
}
