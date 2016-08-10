#include <VoteCollector.h>
#include <HostEntry.h>
#include <boost/log/trivial.hpp>

VoteCollector::VoteCollector(HostList hl){
  // Get all host ids
  for( HostEntry& h : hl.getHosts()){
    ids_.insert(h.getNodeId());
  }
}

void
VoteCollector::storeVote(int id){
  // Check if id is there at all
  if(ids_.find(id) == ids_.end()){
    BOOST_LOG_TRIVIAL(error) << "NodeId " << id << " does not exist!";
    return;
  }
  // Check if id is there already!
  if(votes_.find(id) == votes_.end()){
    votes_.insert(id);
    BOOST_LOG_TRIVIAL(info) << "NodeId " << id << " voted for me";
  } else {
    BOOST_LOG_TRIVIAL(info) << "NodeId " << id << " already exists!";
  }
}

bool
VoteCollector::isElected() const {
  return (votes_.size() > ids_.size()/2) ? true : false;
}

void
VoteCollector::clearVotes(){
  votes_.clear();
}
