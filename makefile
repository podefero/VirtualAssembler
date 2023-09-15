CXX = g++
CXXFLAGS = -std=c++14

all: main

main: main.cpp binary_converter.cpp binary_converter.h
	$(CXX) $(CXXFLAGS) -o main main.cpp binary_converter.cpp

clean:
	rm -f main

