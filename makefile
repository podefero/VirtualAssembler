CXX = g++
CXXFLAGS = -std=c++14

all: binary_converter

binary_converter: main.cpp binary_converter.cpp binary_converter.h
	$(CXX) $(CXXFLAGS) -o binary_converter main.cpp binary_converter.cpp

clean:
	rm -f binary_converter

