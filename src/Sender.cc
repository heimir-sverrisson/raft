#include <Sender.h>
#include <Config.h>
#include <boost/log/trivial.hpp>
#include <UDPSocket.h>
#include <Receiver.h>

void
Sender::sendVoteResponse(ServerState& ss, int nodeId, int vote){
  char sep = Config::messageSeparator;
  BOOST_LOG_TRIVIAL(info) << "Sender: Sending VoteResponse";
  HostEntry h = ss.getHostList().getHostById(nodeId);
  UDPSocket sock(h.getHost(), h.getService(), clientSocket);
  VoteResponse vr(ss.getMyId(), ss.getTerm(), vote);
  string s = to_string(MessageType::voteResponse) + sep + vr.to_string();
  sock.send(s);
  sock.close();
}

void
Sender::sendRequestVote(ServerState& ss){
  int myId = ss.getMyId();
  char sep = Config::messageSeparator;
  RequestVote rv(ss.getTerm(), myId, ss.getLastLogIndex(), ss.getLastLogTerm());
  for(HostEntry& h : ss.getHostList().getHosts()){
    if(h.getNodeId() == myId) // Never send to myself
      continue;
    UDPSocket sock(h.getHost(), h.getService(), clientSocket);
    string s = to_string(MessageType::requestVote) + sep + rv.to_string();
    sock.send(s);
    sock.close();
  }
  BOOST_LOG_TRIVIAL(info) << "Sender: Sent RequestVote to all";
}

void
Sender::sendAppendEntries(ServerState& ss){
  char sep = Config::messageSeparator;
  BOOST_LOG_TRIVIAL(info) << "Sender: Sending AppendEntries to all";
  AppendEntries ae(ss.getTerm(), ss.getMyId(), 1, 2, 3 ); // TODO: correct params needed
  string s = to_string(MessageType::appendEntries) + sep + ae.to_string();
  for(HostEntry& h : ss.getHostList().getHosts()){
    if(h.getNodeId() == ss.getMyId()) // Never send to myself
      continue;
    UDPSocket sock(h.getHost(), h.getService(), clientSocket);
    sock.send(s);
    sock.close();
  }
}

