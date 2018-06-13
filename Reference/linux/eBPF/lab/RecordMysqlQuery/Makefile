
CXX = g++

CXX_FLAGS = -std=c++11

CXX_INCLUDES = -I/usr/lib/llvm-3.7/include -I/usr/include/bcc

CXX_LIB = -lbcc

RecordMySQLQuery : RecordMySQLQuery.o
	$(CXX) $(CXX_FLAGS) -o RecordMySQLQuery RecordMySQLQuery.o $(CXX_LIB)

RecordMySQLQuery.o : RecordMySQLQuery.cc
	$(CXX) $(CXX_FLAGS) $(CXX_INCLUDES) -o RecordMySQLQuery.o -c RecordMySQLQuery.cc

.PHONY : clean
clean :
	rm RecordMySQLQuery RecordMySQLQuery.o
