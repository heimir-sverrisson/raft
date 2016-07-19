#include <Dispatcher.h>
#include <boost/log/trivial.hpp>
#include <RequestVote.h>
#include <AppendEntries.h>
#include <Config.h>

void
Dispatcher::run(Receiver& r, ServerState& ss){
  // Receiver must already be running at this point
  if(!r.isRunning()){
    string str = "Receiver was not running!";
    BOOST_LOG_TRIVIAL(error) << str;
    throw(str);
  }
  srand(ss.getMyId() * getpid());
  int timeout = Config::readPeriod;
  BOOST_LOG_TRIVIAL(info) << "Dispatcher starting";
  while(true){
    // Wait here for either timeout or a message from our beloved leader
    // some candidate that wants to get elected or some poor client
    // that thinks we are the leader.
    WakeupType wake = r.waitForMessage(timeout);
    switch(wake){
      case timedOut: {
          NodeState myState = ss.getNodeState();
          BOOST_LOG_TRIVIAL(info) << "Timeout in NodeState: " << myState;
          switch(myState){
            // Follower geting timeout will make him a Candidate
            case follower: {
                ss.setNodeState(candidate);  // Next state is candidate
                ss.setCandidateState(waitForTimeout);
                VoteCollector& vc = ss.getVoteCollector();
                vc.clearVotes();
                vc.storeVote(ss.getMyId());       // Vote for myself
                timeout = (Config::readPeriod * (rand() % 100)) / 100;
                BOOST_LOG_TRIVIAL(info) << "Candidate timeout: " << timeout;
              }
              break;
            // Candiate getting a timeout means nobody sent a RequestVote
            case candidate:
              switch(ss.getCandidateState()){
                case waitForTimeout:
                  ss.setTerm(ss.getTerm() + 1); // Increment my termId
                  timeout = Config::readPeriod;
                  ss.setCandidateState(waitForVotes);
                  sendRequestVote(ss, r);
                  break;
                case waitForVotes: // Timed out waiting for Votes
                  // Start another election
                  timeout = (Config::readPeriod * (rand() % 100)) / 100;
                  ss.setCandidateState(waitForTimeout);
                  break;
              }
              break;
            // A leader should not experience timeouts (less he is in trouble)
            case leader:
              BOOST_LOG_TRIVIAL(info) << "I am the leader!";
              return; // TODO: fix!
              break;
            default:
              BOOST_LOG_TRIVIAL(error) << "Illegal State: " << myState;
          }
        }
        break;
      case gotMessage:
        while(r.getCount(MessageType::requestVote) > 0){
          RequestVote rv = r.getRequestVote();
          BOOST_LOG_TRIVIAL(info) << rv.to_string();
          handleRequestVote(rv, ss);
          if(ss.getNodeState() == follower){
            BOOST_LOG_TRIVIAL(info) << "I am the loser!";
            return; // TODO: fix
          }
        }
        while(r.getCount(MessageType::appendEntries) > 0){
          AppendEntries ae = r.getAppendEntries();
          BOOST_LOG_TRIVIAL(info) << ae.to_string();
          handleAppendEntries(ae, ss);
          BOOST_LOG_TRIVIAL(info) << "I am the follower!";
          return; // TODO: fix!
        }
        while(r.getCount(voteResponse) > 0){
          VoteResponse vr = r.getVoteResponse();
          BOOST_LOG_TRIVIAL(info) << vr.to_string();
          handleVoteResponse(vr, ss);
        }
        while(r.getCount(MessageType::client) > 0){
          BOOST_LOG_TRIVIAL(info) << "Got a stray client message!";
          handleClientMessage();
        }
        break;
      case notRunning:
        BOOST_LOG_TRIVIAL(error) << "Receiver is dead!";
        throw("Receiver is dead!");
      default:
        BOOST_LOG_TRIVIAL(error) << "Illegal WakeupType: " << wake;
    } // switch(wake)
  } // while(true);
  BOOST_LOG_TRIVIAL(info) << "Dispatcher ending";
}

void
Dispatcher::handleRequestVote(RequestVote& rv, ServerState& ss){
  int theirTerm = rv.get_term();
  if(theirTerm < ss.getTerm()){
    sendVoteResponse(rv, ss, 0);
  } else {
    sendVoteResponse(rv, ss, 1);
    ss.setTerm(theirTerm);
    ss.setNodeState(follower);
  }
}

void
Dispatcher::sendVoteResponse(RequestVote& rv, ServerState& ss, int vote){
  char sep = Config::messageSeparator;
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Sending VoteResponse";
  int candidate = rv.get_candidateId();
  HostEntry h = ss.getHostList().getHostById(candidate);
  UDPSocket sock(h.getHost(), h.getService(), clientSocket);
  VoteResponse vr(ss.getMyId(), ss.getTerm(), vote);
  string s = to_string(MessageType::voteResponse) + sep + vr.to_string();
  sock.send(s);
  sock.close();
}

void
Dispatcher::sendRequestVote(ServerState& ss, Receiver& r){
  int myId = ss.getMyId();
  char sep = Config::messageSeparator;
  RequestVote rv(ss.getTerm(), myId, ss.getLastLogIndex(), ss.getLastLogTerm());
  for(HostEntry& h : ss.getHostList().getAllHosts()){
    if(h.getNodeId() == myId) // Never send myself
      continue;
    UDPSocket sock(h.getHost(), h.getService(), clientSocket);
    string s = to_string(MessageType::requestVote) + sep + rv.to_string();
    sock.send(s);
    sock.close();
  }
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Sent RequestVote";
}


void
Dispatcher::handleVoteResponse(VoteResponse& vr, ServerState& ss){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a VoteResponse";
  if(vr.getVoteGranted() == 1) {
    VoteCollector& vc = ss.getVoteCollector();
    vc.storeVote(vr.getNodeId());
    if(vc.isElected()){
      ss.setNodeState(leader);
      sendAppendEntries(ss, vr.getNodeId());
    }
  }
}

void
Dispatcher::sendAppendEntries(ServerState& ss, int nodeId){
  char sep = Config::messageSeparator;
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Sending AppendEntries to: " << nodeId;
  HostEntry h = ss.getHostList().getHostById(nodeId);
  UDPSocket sock(h.getHost(), h.getService(), clientSocket);
  AppendEntries ae(ss.getTerm(), ss.getMyId(), 1, 2, 3 ); // TODO: correct params needed
  string s = to_string(MessageType::appendEntries) + sep + ae.to_string();
  sock.send(s);
  sock.close();
}

void
Dispatcher::handleAppendEntries(AppendEntries& ae, ServerState& ss){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a AppendEntries";
}

void
Dispatcher::handleClientMessage(){
  BOOST_LOG_TRIVIAL(info) << "Dispatcher: Handling a ClientMessage";
}
