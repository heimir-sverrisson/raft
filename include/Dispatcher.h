#ifndef Dispatcher_h
#define Dispatcher_h

#include <Receiver.h>
#include <ServerState.h>
#include <Sender.h>

using namespace std;

class Dispatcher{
  public:
    void run(Receiver& r, ServerState& ss);
  private:
    int timeout_;
    Sender s_;
    void handleRequestVote(RequestVote& rv, ServerState& ss);
    void handleVoteResponse(VoteResponse& vr, ServerState& ss);
    void handleAppendEntries(AppendEntries& ae, ServerState& ss);
    void handleClientMessage();
};

#endif
