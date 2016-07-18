#include <Candidate.h>
#include <stdlib.h>
#include <time.h>
#include <Config.h>
#include <boost/log/trivial.hpp>
#include <string>
#include <RequestVote.h>
#include <UDPSocket.h>

Candidate::Candidate(){
  // Initialize the random number generator
  srand(time(NULL));
}

//
// If timeout we stay in the Candidate state for new elections
// If I get majority of the votes I become a leader
// If I discover a current leader or new term I become a follower
// 
NodeState
Candidate::run(Receiver& r, ServerState& ss){
  while(true){
    // Wait for a random interval before sending RequestVote message
    // to all the other nodes
    int voteWait = (Config::readPeriod * (rand() % 100)) / 100;
    this_thread::sleep_for(chrono::milliseconds(voteWait));
    // Check to see if we got a vote request from somebody else
    // if we do then we don't need to send our own
    if(r.getCount(requestVote) > 0){
      BOOST_LOG_TRIVIAL(info) << "Got a vote request while waiting!";
      // Send an ack to first candidate - flush other
      return follower;
    }
    // Make sure we have no old messages
    while(r.getCount(voteResponse) > 0){
      r.getVoteResponse();
      BOOST_LOG_TRIVIAL(info) << "Unexpected vote responses found!";
    }
    // No request from others - send them a vote request
    sendRequestVote(ss, r);
    r.waitForMessage();
    bool gotVotes = false;
    while(r.getCount(voteResponse) > 0){
      gotVotes = true;
      VoteResponse vr = r.getVoteResponse();
      BOOST_LOG_TRIVIAL(info) << vr.to_string();
      r.waitForMessage();
    }
    if(gotVotes)
      return leader;
    else {
      BOOST_LOG_TRIVIAL(info) << "Got no votes :-(";
      return follower;
    }
  }
}

void
Candidate::sendRequestVote(ServerState& ss, Receiver& r){
  int myId = ss.getMyId();
  char sep = Config::messageSeparator;
  RequestVote rv(ss.getTerm(), myId, ss.getLastLogIndex(), ss.getLastLogTerm());
  for(HostEntry& h : ss.getHostList().getAllHosts()){
    if(h.getNo() == myId) // Never send myself
      continue;
    UDPSocket sock(h.getHost(), h.getService(), clientSocket);
    string s = to_string(MessageType::requestVote) + sep + rv.to_string();
    sock.send(s);
    sock.close();
  }
}
