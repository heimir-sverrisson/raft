.SUFFIXES: .cc .o .h
.PHONY: all clean
CXX=g++
CXXFLAGS=-g -std=c++11 -pthread -Wall -pedantic -DBOOST_LOG_DYN_LINK \
	 -I/usr/local/include -I../include
BIN := UnitAppendEntries UnitUDPSocket UnitKeyValueStore
LDLIBS = -lboost_unit_test_framework -lboost_log -lboost_log_setup \
       -lboost_thread -lboost_system -lpthread

SRC = UnitAppendEntries.cc 
OBJ=$(SRC:%.cc=%.o)
SOBJ=../src/raft_util.o

all: $(BIN)

UnitAppendEntries: UnitAppendEntries.o ../src/AppendEntries.o 
UnitUDPSocket: UnitUDPSocket.o ../src/UDPSocket.o 
UnitKeyValueStore: UnitKeyValueStore.o ../src/KeyValueStore.o 

$(BIN):
	$(CXX) $(SOBJ) $^ $(LDLIBS) -o $@

.cc.o:
	$(CXX) -c $(CXXFLAGS) -o $@  $<

clean:
	rm -f *.o
	rm -f $(BIN)
