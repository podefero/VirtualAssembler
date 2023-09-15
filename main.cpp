#include "binary_converter.h"
#include <iostream>
#include <string>

using namespace std;
// bool isValid(std::string input);

int main() {
  cout << "Enter a positive, base-10 integer between 0 and 4294967295 "
          "(with no commas) and hit Enter/Return: ";

  string input;
  cin >> input;

  // validate input
  if (!is_valid_input(input)) {
    cerr << "Invalid input. Please enter a positive integer without commas."
         << endl;
    return 1;
  }

  // convert string to int
  int num = string_to_int(input);

  // validate if int is in range
  if (num == -1) {
    cerr << "Invalid range. Please enter a positive integer between 0 and "
            "4294967295."
         << endl;
    return 1;
  }

  // change int into binary string
  string binary = int_to_binary(num);

  cout << "Binary representation: " << binary << endl;

  return 0;
}
