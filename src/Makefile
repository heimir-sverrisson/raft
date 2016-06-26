CXX=g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic -I.
BIN=../bin/raft

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

lint:
	cppcheck .
clean:
	rm -f *.o
	rm $(BIN)
