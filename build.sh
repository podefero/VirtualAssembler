#!/bin/bash
# Nathaniel Jones
# Builds main project and test
# /build will have the main file to run /build/test will have test file to run

# Delete existing build directory if it exists
rm -rf build

# Make build directory
mkdir build

# Change to build directory
cd build

# Use cmake to configure
cmake ..

# Build the project
make

# Move the test file
cp ../tests/we.bin tests
cp ../tests/we.asm tests
