# Project 3

## Build Instructions

To build tests and main program in the root directory:

```bash
./build.sh
```
This will create a `build` folder that contains the following:

- The main executable `main_prog` in the `build/` directory
- The test executable `tests` in the `build/tests` directory

## Running the Main Program

To run the main program, follow these steps:

1. Navigate to the `build` directory:

```bash
cd build
```
2. Execute the main program with a file path as an argument:

```bash
./main_prog <filePath>
```

```bash 
#To run assembly portion for P3
./main_prog ../asm/Project3_OddsEvens.asm
./main_prog ../asm/Project3_DagsGads.asm
```

```bash
#To run binary file
./main_prog ../asm/Project3_OddsEvens.bin
./main_prog ../asm/Project3_DagsGads.bin
```

## Running Tests

To run the tests, assuming you are in the `build` folder, use the following commands:

```bash
cd tests
./tests
```

## Change Log
- Fixed TRP4 and TRP 2, swapped the logic on those instructions
- Fixed bug where asm file would throw error on comment line with white space before it.
- Cleaned up redundant code. 
- Tokens.h is more simplified 
- Dump registers and symbol table on errors. 
- Removed all .cpp files for operation and tokens. Combined in .h file.
- Made code more readable, using enum for opcodes now. 
- Fixed .BYT Directives to now support special characters
- Simplified vm_tests.cpp. Due to the nature of exception handling it's a little backwards
  - Will need to adjust so all exception throws will be handled by main.
- Added instructions from P3