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
    int m_timeout;
    Sender m_s;
    void handleRequestVote(RequestVote& rv, ServerState& ss);
    void handleVoteResponse(VoteResponse& vr, ServerState& ss);
    void handleAppendEntries(AppendEntries& ae, ServerState& ss);
    void handleClientMessage();
};

#endif
