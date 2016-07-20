#ifndef Dispatcher_h
#define Dispatcher_h

#include <Receiver.h>
#include <ServerState.h>

using namespace std;

class Dispatcher{
  public:
    void run(Receiver& r, ServerState& ss);
  private:
    int m_timeout;
    void handleRequestVote(RequestVote& rv, ServerState& ss);
    void handleVoteResponse(VoteResponse& vr, ServerState& ss);
    void handleAppendEntries(AppendEntries& ae, ServerState& ss);
    void handleClientMessage();
    void sendRequestVote(ServerState& ss, Receiver& r);
    void sendVoteResponse(RequestVote& rv, ServerState& ss, int vote);
    void sendAppendEntries(ServerState& ss);
};

#endif
