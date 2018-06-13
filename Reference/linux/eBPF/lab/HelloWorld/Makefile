
CXX = g++

CXX_FLAGS = -std=c++11

CXX_INCLUDES = -I/usr/lib/llvm-3.7/include -I/usr/include/bcc

CXX_LIB = -lbcc

HelloWorld : HelloWorld.o
	$(CXX) $(CXX_FLAGS) -o HelloWorld HelloWorld.o $(CXX_LIB)

HelloWorld.o : HelloWorld.cc
	$(CXX) $(CXX_FLAGS) $(CXX_INCLUDES) -o HelloWorld.o -c HelloWorld.cc

.PHONY : clean
clean :
	rm HelloWorld HelloWorld.o
