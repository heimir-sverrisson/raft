.PHONY: all lint clean
CXX=g++
CXXFLAGS=-g -std=c++11 -pthread -Wall -pedantic -DBOOST_LOG_DYN_LINK \
	 -I/usr/local/include -I../include
BIN=../bin/raft
LDLIBS=-lboost_log -lboost_log_setup -lboost_system -lboost_thread -lpthread

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)
DEP=$(SRC:%.cc=%.d)

all: $(BIN)

RaftProtocol: RaftProtocol.o Receiver.o UDPSocket.o AppendEntries.o raft_util.o RequestVote.o VoteResponse.o
	$(CXX) $(LDLIBS) $^ -o $@

$(BIN): $(OBJ)
	$(CXX) $(LDLIBS) $^ -o $@

lint:
	cppcheck .
clean:
	rm -f $(OBJ) $(DEP)

%.d: %.cc
	$(CXX) -MM $(CXXFLAGS) $< > $@

-include $(DEP)
