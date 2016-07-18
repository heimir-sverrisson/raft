#ifndef Dispatcher_h
#define Dispatcher_h

#include <Receiver.h>
#include <ServerState.h>

using namespace std;

class Dispatcher{
  public:
    void run(Receiver& r, ServerState& ss);
  private:
    void handleRequestVote(RequestVote& rv, ServerState& ss);
    void handleVoteResponse(VoteResponse& vr, ServerState& ss);
    void handleAppendEntries(AppendEntries& ae, ServerState& ss);
    void handleClientMessage();
    void sendVoteResponse(RequestVote& rv, ServerState& ss, int vote);
};

#endif
