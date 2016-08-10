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
    bool isElected() const;
    void clearVotes();
  private:
    int hostCount_;
    unordered_set<int> ids_;
    unordered_set<int> votes_;
};

#endif
