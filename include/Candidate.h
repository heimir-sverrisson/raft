#ifndef Candidate_h
#define Candidate_h

#include <Receiver.h>
#include <ServerState.h>

using namespace std;

class Candidate {
  public:
    Candidate();
    NodeState run(Receiver& r, ServerState& ss);
  private:
    void sendRequestVote(ServerState& ss, Receiver& r);
};

#endif
