cmake_minimum_required(VERSION 3.12)
project(virtualmachine)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Add the source files to your project
add_executable(main_prog src/main.cpp)

# Include the mpark library
target_include_directories(main_prog PRIVATE include)

# Link the mpark library (assuming it's a header-only library)
# You may not need to link anything if it's header-only.
# Include the subdirectory for tests
add_subdirectory(tests)

