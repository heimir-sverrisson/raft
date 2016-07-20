.PHONY: all lint clean
CXX=g++
CXXFLAGS=-g -std=c++11 -pthread -Wall -pedantic -DBOOST_LOG_DYN_LINK \
	 -I/usr/local/include -I../include
OUTPUT_OPTION=-MMD -MP -o $@
BIN=../bin/raft
LDLIBS=-lboost_log -lboost_log_setup -lboost_system -lboost_thread -lpthread

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)
DEP=$(SRC:%.cc=%.dep)

all: $(OBJ)
	$(CXX) $(LDLIBS) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

lint:
	cppcheck .
clean:
	rm -f $(OBJ) $(DEP)

%.dep: %.cc
	$(CXX) -MM $(CXXFLAGS) $< > $@

-include $(DEP)
