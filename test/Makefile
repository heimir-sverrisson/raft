.SUFFIXES: .cc .o .h
.PHONY: all clean
CXX=g++
CXXFLAGS=-g -std=c++11 -pthread -Wall -pedantic -DBOOST_LOG_DYN_LINK \
	 -I/usr/local/include -I../include
BIN := UnitAppendEntries
LDLIBS = -lboost_unit_test_framework -lboost_log -lboost_log_setup \
       -lboost_thread -lpthread

SRC = UnitAppendEntries.cc 
OBJ=$(SRC:%.cc=%.o)
SOBJ=../src/AppendEntries.o

all: $(BIN)

$(BIN):	$(OBJ) $(SOBJ)
	$(CXX) $(SOBJ) $(OBJ) $(LDLIBS) -o $(BIN)

.cc.o:
	$(CXX) -c $(CXXFLAGS) -o $@  $<

clean:
	rm -f *.o
	rm -f $(BIN)
