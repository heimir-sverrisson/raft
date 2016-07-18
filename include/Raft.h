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
    ServerState m_ss;  
    Receiver m_r;
    thread m_rcv;
    void receiveWorker();
};

#endif
