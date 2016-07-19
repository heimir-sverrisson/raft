#ifndef VoteCollector_h
#define VoteCollector_h

#include <HostList.h>
#include <unordered_set>

using namespace std;

// Collect votes during a leader election
//
class VoteCollector{
  public:
    VoteCollector(HostList hl);
    void storeVote(int id);
    bool isElected();
    void clearVotes();
  private:
    int m_hostCount;
    unordered_set<int> m_ids;
    unordered_set<int> m_votes;
};

#endif
