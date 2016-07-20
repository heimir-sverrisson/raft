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
    inline HostList getHostList() { return m_hostList; };
    inline NodeState getNodeState() { return m_nodeState; };
    inline CandidateState getCandidateState() { return m_candidateState; };
    inline int getTerm() { return m_term; };
    inline int getMyId() { return m_myId; };
    inline int getLastLogIndex() { return m_lastLogIndex; };
    inline int getLastLogTerm() { return m_lastLogTerm; };
    inline VoteCollector& getVoteCollector() { return m_voteCollector; };
    inline void setNodeState(NodeState nodeState) { m_nodeState = nodeState; };
    inline void setCandidateState(CandidateState candidateState) { m_candidateState = candidateState; };
    inline void setTerm(int term) { m_term = term; };
    inline void setLastLogIndex(int lastLogIndex) { m_lastLogIndex = lastLogIndex; };
    inline void setLastLogTerm(int lastLogTerm) { m_lastLogTerm = lastLogTerm; };
  private:
    HostList m_hostList;
    NodeState m_nodeState;
    CandidateState m_candidateState;
    int m_term;
    int m_myId;
    int m_lastLogIndex;
    int m_lastLogTerm;
    VoteCollector m_voteCollector;
};

inline ServerState::ServerState(HostList hostList, int myId)
  : m_hostList(hostList), 
    m_nodeState(follower),
    m_term(0),
    m_myId(myId),
    m_lastLogIndex(0),
    m_lastLogTerm(0),
    m_voteCollector(hostList) 
{}

#endif
