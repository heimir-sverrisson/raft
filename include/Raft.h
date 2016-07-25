#ifndef Raft_h
#define Raft_h

#include <string>
#include <Receiver.h>
#include <ServerState.h>

using namespace std;

class Raft{
  public:
    Raft(string configFile, int myId);
    void run();
  private:
    ServerState ss_;  
    Receiver r_;
    thread rcv_;
    void receiveWorker();
};

#endif
