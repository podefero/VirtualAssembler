# Project 5

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
#To run assembly portion for P5
./main_prog ../asm/P5.asm
```

```bash
#To run binary file
./main_prog ../asm/P5.bin
```

## Running Tests

To run the tests, assuming you are in the `build` folder, use the following commands:

```bash
cd tests
./tests

# Integrated Test automates previous projects and my own tests
# This will run most files in the ../asm/test/ directory
./integratedTest
```
