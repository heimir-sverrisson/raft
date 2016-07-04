CXX=g++
CXXFLAGS=-g -std=c++11 -pthread -Wall -pedantic -DBOOST_LOG_DYN_LINK \
	 -I/usr/local/include -I../include
BIN=../bin/raft
LDLIBS=-lboost_log -lboost_log_setup -lboost_thread -lpthread

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
	rm -f $(BIN)
