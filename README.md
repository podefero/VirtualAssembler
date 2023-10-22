# Project 2

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

## Video Link

You can watch a video related to this project by clicking on the following link:

[Watch Video](https://drive.google.com/file/d/11tAhgiXCJ_F3gqOc40woM6_n_m6zLH2t/view?usp=sharing)
