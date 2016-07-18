#ifndef ServerState_h
#define ServerState_h

#include <HostList.h>

using namespace std;

enum NodeState {
  follower, candidate, leader
};

class ServerState {
  public:
    ServerState(HostList hostList, int myId);
    inline HostList getHostList() { return m_hostList; };
    inline NodeState getNodeState() { return m_nodeState; };
    inline int getMyId() { return m_myId; };
    inline int getTerm() { return m_term; };
    inline int getLastLogIndex() { return m_lastLogIndex; };
    inline int getLastLogTerm() { return m_lastLogTerm; };
    inline void setNodeState(NodeState nodeState) { m_nodeState = nodeState; };
    inline void setTerm(int term) { m_term = term; };
    inline void setLastLogIndex(int lastLogIndex) { m_lastLogIndex = lastLogIndex; };
    inline void setLastLogTerm(int lastLogTerm) { m_lastLogTerm = lastLogTerm; };
  private:
    HostList m_hostList;
    NodeState m_nodeState;
    int m_term;
    int m_myId;
    int m_lastLogIndex;
    int m_lastLogTerm;
};

inline ServerState::ServerState(HostList hostList, int myId)
  : m_hostList(hostList), 
    m_nodeState(follower),
    m_term(0),
    m_myId(myId),
    m_lastLogIndex(0),
    m_lastLogTerm(0) 
{}

#endif
