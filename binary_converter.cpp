#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::cout << "Enter a positive, base-10 integer between 0 and 4294967295 (with no commas) and hit Enter/Return: ";

    std::string input;
    std::cin >> input;

    // Validate input
    for (char c : input) {
        if (!isdigit(c)) {
            std::cerr << "Invalid input. Please enter a positive integer without commas." << std::endl;
            return 1;
        }
    }

    // Convert the input string to an integer
    unsigned int num;
    std::istringstream iss(input);
    if (!(iss >> num) || num < 0 || num > 4294967295) {
        std::cerr << "Invalid input. Please enter a positive integer between 0 and 4294967295." << std::endl;
        return 1;
    }

    // Convert the integer to binary and output as a string
    std::string binary;
    while (num > 0) {
        binary = std::to_string(num % 2) + binary;
        num /= 2;
    }

    std::cout << "Binary representation: " << binary << std::endl;

    return 0;
}

