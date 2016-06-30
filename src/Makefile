CXX=g++
CXXFLAGS=-g -std=c++11 -pthread -Wall -pedantic -I/usr/local/include -I.
BIN=../bin/raft
LDLIBS=-pthread

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)

all: $(OBJ)
	$(CXX) $(LDLIBS) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

lint:
	cppcheck .
clean:
	rm -f *.o
	rm $(BIN)
