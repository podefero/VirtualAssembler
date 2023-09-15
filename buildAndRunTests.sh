#!/bin/bash
# Nathaniel Jones
# Automate building cmake stuff
cmake -S . -B build
read -p "Press Enter to continue.."
cmake --build build
read -p "Press Enter to test.."
cd build && ctest
