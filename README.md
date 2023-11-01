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
    - In this case for Project 2 use the HelloWord.asm as argument, below is instructions.

```bash
./main_prog <filePath>

#To run assembly portion using my Hellworld.asm
./main_prog ../HelloWorld.asm #This will create a output.bin file

#To run binary file
./main_prog outputbin.bin
```

## Running Tests

To run the tests, assuming you are in the `build` folder, use the following commands:

```bash
cd tests
./tests
```

## Change Log
- Fixed TRP4 and TRP 2, swapped the logic on those instructions
- Cleaned up redundant code. 
- Tokens.h is more simplified 
- Dump registers and symbol table on errors. 
- Removed all .cpp files for operation and tokens. Combined in .h file.
- Made code more readable, using enum for opcodes now. 
- Added instructions from P3