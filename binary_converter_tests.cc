#include "binary_converter.h"
#include <gtest/gtest.h>
#include <string>

// Test case for valid input
TEST(IsValidTest, ValidInput) {
  std::string validInput = "12345";
  EXPECT_TRUE(is_valid_input(validInput));
}

// Test case for invalid input
TEST(IsValidTest, InvalidInput) {
  std::string invalidInput = "12a45";
  EXPECT_FALSE(is_valid_input(invalidInput));
}

// Test string to int invalid range
TEST(StringToInt, LargeStrToInt) {
  std::string input = "4294967295";
  int expectedOutput = -1;
  EXPECT_EQ(string_to_int(input), expectedOutput);
}

// Test string to int valid range
TEST(StringToInt, ValidStrToInt) {
  std::string input = "10";
  int expectedOutput = 10;
  EXPECT_EQ(string_to_int(input), expectedOutput);
}

// Test output
TEST(ValidOutput, ValidOutput) {
  // Assume the function under test returns "100"
  std::string actualOutput = int_to_binary(4);
  std::string expectedOutput = "100";

  ASSERT_EQ(expectedOutput, actualOutput);
}
