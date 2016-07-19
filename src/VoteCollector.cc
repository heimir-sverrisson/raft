#include <VoteCollector.h>
#include <HostEntry.h>
#include <boost/log/trivial.hpp>

VoteCollector::VoteCollector(HostList hl){
  // Get all host ids
  for( HostEntry& h : hl.getAllHosts()){
    m_ids.insert(h.getNodeId());
  }
}

void
VoteCollector::storeVote(int id){
  // Check if id is there at all
  if(m_ids.find(id) == m_ids.end()){
    BOOST_LOG_TRIVIAL(error) << "NodeId " << id << " does not exist!";
    return;
  }
  // Check if id is there already!
  if(m_votes.find(id) == m_votes.end()){
    m_votes.insert(id);
    BOOST_LOG_TRIVIAL(error) << "NodeId " << id << " voted for me";
  } else {
    BOOST_LOG_TRIVIAL(info) << "NodeId " << id << " already exists!";
  }
}

bool
VoteCollector::isElected(){
  return (m_votes.size() > m_ids.size()/2) ? true : false;
}

void
VoteCollector::clearVotes(){
  m_votes.clear();
}
