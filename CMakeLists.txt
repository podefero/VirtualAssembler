cmake_minimum_required(VERSION 3.12)
project(virtualmachine)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")

# Add the source files to your project
add_executable(main_prog ${SOURCES} Main.cpp)

# Include my header files
target_include_directories(main_prog PRIVATE include)

# Include the subdirectory for tests
add_subdirectory(tests)

