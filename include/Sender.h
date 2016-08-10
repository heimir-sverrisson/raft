#ifndef Sender_h
#define Sender_h

#include <ServerState.h>
#include <AppendEntries.h>
#include <RequestVote.h>
#include <VoteResponse.h>

using namespace std;

class Sender{
  public:
    void sendAppendEntries(ServerState& ss);
    void sendRequestVote(ServerState& ss);
    void sendVoteResponse(ServerState& ss, int candidateId, int vote);
};

#endif
