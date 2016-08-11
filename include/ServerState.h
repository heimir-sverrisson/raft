#ifndef ServerState_h
#define ServerState_h

#include <HostList.h>
#include <VoteCollector.h>

using namespace std;

// The overall state of the Node
enum NodeState {
  follower, candidate, leader
};
// Substates for Candidate
enum CandidateState {
  waitForTimeout, waitForVotes
};

class ServerState {
  public:
    ServerState(HostList hostList, int myId);
    inline HostList getHostList() const { return hostList_; };
    inline NodeState getNodeState() const { return nodeState_; };
    inline CandidateState getCandidateState() const { return candidateState_; };
    inline int getTerm() const { return term_; };
    inline int getMyId() const { return myId_; };
    inline int getLastLogIndex() const { return lastLogIndex_; };
    inline int getLastLogTerm() const { return lastLogTerm_; };
    inline int getVoteTerm() const { return voteTerm_; };
    inline VoteCollector& getVoteCollector() { return voteCollector_; };
    inline void setNodeState(NodeState nodeState) { nodeState_ = nodeState; };
    inline void setCandidateState(CandidateState candidateState) { candidateState_ = candidateState; };
    inline void setTerm(int term) { term_ = term; };
    inline void setLastLogIndex(int lastLogIndex) { lastLogIndex_ = lastLogIndex; };
    inline void setLastLogTerm(int lastLogTerm) { lastLogTerm_ = lastLogTerm; };
    inline void setVoteTerm(int voteTerm) { voteTerm_ = voteTerm; };
  private:
    HostList hostList_;
    NodeState nodeState_;
    CandidateState candidateState_;
    int term_;
    int myId_;
    int lastLogIndex_;
    int lastLogTerm_;
    int voteTerm_;
    VoteCollector voteCollector_;
};

inline ServerState::ServerState(HostList hostList, int myId)
  : hostList_(hostList), 
    nodeState_(follower),
    term_(0),
    myId_(myId),
    lastLogIndex_(0),
    lastLogTerm_(0),
    voteTerm_(0),
    voteCollector_(hostList) 
{}

#endif
