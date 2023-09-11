CXX = g++
CXXFLAGS = -std=c++11

all: binary_converter

binary_converter: binary_converter.cpp
	$(CXX) $(CXXFLAGS) -o binary_converter binary_converter.cpp

clean:
	rm -f binary_converter

